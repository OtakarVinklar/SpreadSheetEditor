#include "CTextParser.hpp"

bool CTextParser::Parse(const std::string & content, ECell & cellType, double & numberValue, TCell_dep_table & dependencies,
						 std::vector<CToken> & postfixExpr, bool & isNumber, std::string & exception) const {

	// Is string cell
	if (content[0] != '=' && content[0] != '-' && (content[0] < '0' || content[0] > '9')) {
		cellType = ECell::String;
		isNumber = false;
		return true;
	}

	unsigned position = 0;
	if ( (content[0] >= '0' && content[0] <= '9') || content[0] == '-') {	
		// Is number cell
		if (ReadNumber(content, numberValue, position) && position == content.length()) {
			cellType = ECell::Numeric;
			isNumber = true;
		}
		// Is string cell
		else {
			cellType = ECell::String;
			isNumber = false;
		}
		return true;
	}

	std::vector<CToken> infixTokenExpr;

	if (!Tokenize(content, infixTokenExpr)) {
//		exception = "To_Token_Ex";
		exception = "InvalidForm";
		return false;
	}
	if (!ConvertToPostfix(infixTokenExpr, postfixExpr)) {
//		exception = "To_Post_Ex";
		exception = "InvalidForm";
		return false;
	}
	if (!isValidPostfix(postfixExpr)) {
//		exception = "Expr_Val_Ex";
		exception = "InvalidForm";
		return false;
	}
	// is Equation cell
	CalculateDependencies(dependencies, postfixExpr);
	isNumber = true;
	cellType = ECell::Equation;
	return true;
}

bool CTextParser::ReadNumber(const std::string & input, double & number, unsigned & position) const {
	std::stringstream ss(input.substr(position, input.length()));
	ss >> number;
	
	bool decimalPoint = false;
	if (input[position] == '-') {
		++position;
		if (position == input.length())
				return false;
		if (input[position] < '0' || input[position] > '9')
			return false;
	}

	if ((position + 1) != input.length() && input[position] == '0') {
		if (input[position + 1] != '.')
			return false;
		decimalPoint = true;
		position += 2;
	}

	while (position < input.length() && ((input[position] >= '0' && input[position] <= '9') || input[position] == '.')) {
		if (input[position] == '.') {
			if (decimalPoint)
				break;
			decimalPoint = true;
		}
		++position;
	}
	return true;
}

bool CTextParser::ReadIntNumber(const std::string & input, unsigned & number, unsigned & position) const {
	std::stringstream ss(input.substr(position, input.length()));
	ss >> number;

	if ((position + 1) != input.length() && input[position] == '0') {
		if (input[position + 1] >= '0' && input[position + 1] <= '9')
			return false;
	}

	while (position < input.length() && input[position] >= '0' && input[position] <= '9') {
		++position;
	}
	return true;
}

bool CTextParser::Tokenize(const std::string & input, std::vector<CToken> & tokens) const {
	// current position in the currently parsed string
	unsigned pos = 1;
	// previous operator
	CToken prev;
	// posibillity of operand token

	while (pos < input.length()) {
		bool operat = true;
		switch (input[pos]) {
			case  ' ':
				++pos;
				continue;
			case '*':
				tokens.push_back(prev = CToken(EBinaryOp::Multi));
				break;			
			case '/':
				tokens.push_back(prev = CToken(EBinaryOp::Divis));
				break;
			case '+':
				if (prev.m_Bracket != EBracket::Right && (tokens.empty() || prev.getType() == EToken::Operator))
					tokens.push_back(prev = CToken(EUnaryOp::Plus));
				else
					tokens.push_back(prev = CToken(EBinaryOp::Plus));
				break;
			case '-':
				if (prev.m_Bracket != EBracket::Right && (tokens.empty() || prev.getType() == EToken::Operator)) {
					tokens.push_back(prev = CToken(EUnaryOp::Minus));
				}
				else {
					tokens.push_back(prev = CToken(EBinaryOp::Minus));
				}
				break;
			case ')':
				tokens.push_back(prev = CToken(EBracket::Right));
				break;
			case '(':
				tokens.push_back(prev = CToken(EBracket::Left));
				break;
			case '^':
				tokens.push_back(prev = CToken(EBinaryOp::Exp));
				break;
			default:
				operat = false;
		}
		if (operat) {
				++pos;
				continue;
		}
		// reading higher case letters - posibility of cell link token
		if (input[pos] >= 65 && input[pos] <= 90) {
			TPosition cellLink;
			if (!ReadCellLink(input, cellLink, pos))
				return false;
			tokens.push_back(prev = CToken(cellLink));
			continue;
		}
		// reading numbers - posibility of number
		if ( (input[pos] >= '0' && input[pos] <= '9') || input[pos] == '-') {
			double number;
			if (!ReadNumber(input, number, pos)) {
				return false;
			}
			tokens.push_back(prev = CToken(number));
			continue;
		}
		// reading lower case letters - possibility of function token
		if (input[pos] >= 97 && input[pos] <= 122) {
			CToken token;
			if (!ReadFunction(input, token, pos))
				return false;
			tokens.push_back(prev = token);
			continue;
		}

		return false;
	}
	return true;
}

bool CTextParser::ReadCellLink(const std::string & input, TPosition & cellLink, unsigned & position) const {
	unsigned row;
	unsigned column;
	std::string columnLable;

	if (input[position] < 65 || input[position] > 90)
		return false;
	
	while (input[position] >= 65 && input[position] <= 90) {
		if ((position + 1) == input.length())
			return false;
		columnLable.push_back(input[position]);
		++position;
	}
	column = FromColumnAlfabetLabelingToNumber(columnLable);
	
	if (input[position] < '0' || input[position] > '9')
		return false;

	if (!ReadIntNumber(input, row, position))
		return false;

	cellLink = std::make_pair(row, column);
	return true;
}

bool CTextParser::ReadSimpleFunction(const std::string & input, CToken & token, unsigned & pos, EFunSimple function) const {
	token = CToken(function);
	return true;
}

bool CTextParser::ReadAggregateFunction(const std::string & input, std::pair<TPosition, TPosition> & cellLinks, unsigned & pos) const {

	SkipWhiteSpace(input, pos);
	if (pos == input.length() || input[pos] != '(') {
		return false;
	}
	++pos;
	SkipWhiteSpace(input, pos);
	if (!ReadCellLink(input, cellLinks.first, pos)) {
		return false;
	}
	SkipWhiteSpace(input, pos);
	if (pos == input.length() || input[pos] != ':') {
		return false;
	}
	++pos;
	SkipWhiteSpace(input, pos);
	if (!ReadCellLink(input, cellLinks.second, pos))
		return false;
	SkipWhiteSpace(input, pos);
	if (pos == input.length() || input[pos] != ')') 
		return false;
	++pos;
	return true;	
}

bool CTextParser::ReadFunction(const std::string & input, CToken & token, unsigned & pos) const {
/*
possible functions:
simple: ln, log, abs, sin
aggregate: avr, max, sum
*/
	std::string readString;

	std::pair<TPosition, TPosition> cellLinks;

	while (pos < input.length() && input[pos] >= 97 && input[pos] <= 122) {
		readString.push_back(input[pos]);
		++pos;

		unsigned fType;
		for (fType = 0; fType <= FUNCTION_COUNT; ++fType)
			if (FUNCTIONS[fType].compare(readString) == 0)
				break;

		switch (fType) {
			case FUNCTION_LN:
				if (!ReadSimpleFunction(input, token, pos, EFunSimple::Ln))
					return false;
				return true;
			case FUNCTION_LOG:
				if (!ReadSimpleFunction(input, token, pos, EFunSimple::Log))
					return false;
				return true;
			case FUNCTION_ABS:
				if (!ReadSimpleFunction(input, token, pos, EFunSimple::Abs))
					return false;
				return true;
			case FUNCTION_SIN:
				if (!ReadSimpleFunction(input, token, pos, EFunSimple::Sin))
					return false;
				return true;
			case FUNCTION_COS:
				if (!ReadSimpleFunction(input, token, pos, EFunSimple::Cos))
					return false;
				return true;
			case FUNCTION_FLOOR:
				if (!ReadSimpleFunction(input, token, pos, EFunSimple::Floor))
					return false;
				return true;
			case FUNCTION_CEIL:
				if (!ReadSimpleFunction(input, token, pos, EFunSimple::Ceil))
					return false;
				return true;
			case FUNCTION_AVR:
				if (!ReadAggregateFunction(input, cellLinks, pos))
					return false;
				token = CToken(EFunAggregate::Average, cellLinks);
				return true;
			case FUNCTION_MAX:
				if (!ReadAggregateFunction(input, cellLinks, pos))
					return false;
				token = CToken(EFunAggregate::Max, cellLinks);
				return true;
			case FUNCTION_SUM:
				if (!ReadAggregateFunction(input, cellLinks, pos))
					return false;
				token = CToken(EFunAggregate::Sum, cellLinks);
				return true;
		}
	}
	return false;
}

bool CTextParser::ConvertToPostfix(const std::vector<CToken> & infixTokenExpr, std::vector<CToken> & postfixExpr) const {
	std::stack<CToken> operators;
	bool prevOperand = false;

	for (const auto & token : infixTokenExpr) {
		EToken tType = token.getType(); 
		if (tType == EToken::Operand) {

			if (prevOperand)
				return false;
			prevOperand = true;

			postfixExpr.push_back(token);
		}
		else if (tType == EToken::Operator) {
			prevOperand = false;
			switch (token.m_Operator) {
				case EOperator::Bracket: {
					// managing left bracket - left bracket is always put on the stack
					if (token.m_Bracket == EBracket::Left) {
						operators.push(token);
						continue;
					}
					// managing right bracket - corresponding left bracket must be found on the stack! 
					if (operators.empty())
						return false;

					while (operators.top().m_Bracket != EBracket::Left) {
						postfixExpr.push_back(operators.top());
						operators.pop();
						if (operators.empty())
							return false;
					}
					operators.pop();
					continue;
				}
				case EOperator::Unary:
					operators.push(token);
					continue;	
				case EOperator::Binary: {
					// if operator on the stack has lower priority, put it on the stack 
					while ( (!operators.empty()) && (token.getOperatorPriority() <= operators.top().getOperatorPriority() ) ) {
						postfixExpr.push_back(operators.top());
						operators.pop();
					}
					operators.push(token);
					continue;						
				}
				default:
					return false;
			} // switch token.m_Operator end
		} // else if end
		else
			return false;
	}
	while (!operators.empty()) {
		postfixExpr.push_back(operators.top());
		operators.pop();
	}

	return true;
}

unsigned CTextParser::FromColumnAlfabetLabelingToNumber(std::string label) const {
	std::vector<unsigned> numbers;

	for (int i = label.length() - 1; i >= 0; --i) {
		numbers.push_back(label[i] - 65);
	}
	if (numbers.size() > 1)
		numbers[numbers.size() - 1]++;

	unsigned result = 0, numberToAdd;

	for (unsigned i = 0; i < numbers.size(); ++i) {
		numberToAdd = numbers[i];
		for (unsigned j = 0; j < i; ++j)
			numberToAdd *= 26;
		result += numberToAdd;
	}
	return result;
}

void CTextParser::SkipWhiteSpace(const std::string input, unsigned & pos) const {
	while (pos != input.length() && input[pos] == ' ')
		++pos;
}

void CTextParser::CalculateDependencies(TCell_dep_table & dependencies, const std::vector<CToken> & postfixExpr) const {
	for (const auto & token : postfixExpr) {
		if (token.getType() == EToken::Operand) {
			switch (token.m_Operand) {
				case EOperand::CellLink :
						dependencies.insert(std::make_pair(token.m_CellLink, 0));
					break;
				case EOperand::AFunction :
					PutAggregateFunDep(token, dependencies);
					break;
				default:
					break;
			} // switch EToken::Operand 
		} // if operand end
	} // for cycle end
} // method end

void CTextParser::PutAggregateFunDep(const CToken & token, TCell_dep_table & dependencies) const {
	std::vector<std::pair<TPosition, double>> depFromToken;

	unsigned rowMin = token.m_CellLinks.first.first;
	unsigned colMin = token.m_CellLinks.first.second;

	unsigned rowMax = token.m_CellLinks.second.first;
	unsigned colMax = token.m_CellLinks.second.second;

	for (unsigned row = rowMin; row <= rowMax; ++row)
		for (unsigned column = colMin; column <= colMax; ++ column)
			depFromToken.push_back(std::make_pair(std::make_pair(row, column), 0));

	dependencies.insert(depFromToken.begin(), depFromToken.end());
}

bool CTextParser::isValidPostfix(const std::vector<CToken> & postfix) const {
	unsigned operandCount = 0;
	for (const auto & token : postfix) {
		if (token.getType() == EToken::Operand) {
			++operandCount;
			continue;
		}
		if (token.getType() == EToken::Operator) {
			if (token.m_Operator == EOperator::Unary && operandCount < 1) {
				return false;
			}
			if (token.m_Operator == EOperator::Binary && operandCount-- < 2) {
				return false;
			}
			continue;
		}
		return false;
	}
	if (operandCount != 1) {
		return false;
	}
	return true;
}
