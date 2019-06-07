#include "CTokenExpEvaluator.hpp"


double CTokenExpEvaluator::Evaluate (const std::vector<CToken> & expr, const TCell_dep_table & depend) const {
	std::stack<double> operands;
	
	for (unsigned pos = 0; pos < expr.size(); ++pos) {
		if (expr[pos].getType() == EToken::Operand)
			operands.push(getOperandValue(expr[pos], depend));
		if (expr[pos].getType() == EToken::Operator)
			CalculateOperation(operands, expr[pos]);
	}
	return operands.top();
}

double CTokenExpEvaluator::getOperandValue(const CToken & token, const TCell_dep_table & depend) const {
	switch (token.m_Operand) {
		case EOperand::Number:
			return token.m_NumValue;
		case EOperand::CellLink:
			return depend.find(token.m_CellLink)->second;
		case EOperand::AFunction:
			return getFunctionValue(token, depend);
		default:
			return 0; 
	}
}

double CTokenExpEvaluator::getFunctionValue(const CToken & token, const TCell_dep_table & depend) const {
			switch (token.m_FunAggregate)	{
				case EFunAggregate::Max:
					return CalculateFunctionMax(token, depend);
				case EFunAggregate::Sum:
					return CalculateFunctionSum(token, depend);
				case EFunAggregate::Average:
					return CalculateFunctionAverage(token, depend);
				default:
					return 0;
			}
}

double CTokenExpEvaluator::CalculateFunctionMax(const CToken & token, const TCell_dep_table & depend) const {
	unsigned rowMin = token.m_CellLinks.first.first;
	unsigned colMin = token.m_CellLinks.first.second;

	unsigned rowMax = token.m_CellLinks.second.first;
	unsigned colMax = token.m_CellLinks.second.second;
	double max = depend.find(token.m_CellLinks.first)->second;
	double current;

	for (unsigned row = rowMin; row <= rowMax; ++row)
		for (unsigned column = colMin; column <= colMax; ++ column)
			if ( (current = depend.find(std::make_pair(row, column))->second) > max )
				max = current;

	return max;
}

double CTokenExpEvaluator::CalculateFunctionSum(const CToken & token, const TCell_dep_table & depend) const {
	unsigned rowMin = token.m_CellLinks.first.first;
	unsigned colMin = token.m_CellLinks.first.second;

	unsigned rowMax = token.m_CellLinks.second.first;
	unsigned colMax = token.m_CellLinks.second.second;
	double sum = 0;

	for (unsigned row = rowMin; row <= rowMax; ++row)
		for (unsigned column = colMin; column <= colMax; ++ column)
			sum += depend.find(std::make_pair(row, column))->second;

	return sum;
}

double CTokenExpEvaluator::CalculateFunctionAverage(const CToken & token, const TCell_dep_table & depend) const {
	unsigned rowMin = token.m_CellLinks.first.first;
	unsigned colMin = token.m_CellLinks.first.second;

	unsigned rowMax = token.m_CellLinks.second.first;
	unsigned colMax = token.m_CellLinks.second.second;
	double avr = 0;
	double count = 0;

	for (unsigned row = rowMin; row <= rowMax; ++row)
		for (unsigned column = colMin; column <= colMax; ++ column) {
			double current = depend.find(std::make_pair(row, column))->second;
			avr = (avr * count + current)/(count + 1) ;
			++count;
		}

	return avr;
}

void CTokenExpEvaluator::CalculateOperation(std::stack<double> & operands, const CToken & token) const {
	if (token.m_Operator == EOperator::Unary)
		CalculateUnaryOperation(operands, token);
	if (token.m_Operator == EOperator::Binary)
		CalculateBinaryOperation(operands, token);
}

void CTokenExpEvaluator::CalculateBinaryOperation(std::stack<double> & operands, const CToken & token) const {
	double rightOperand = operands.top();
	operands.pop();
	double leftOperand = operands.top();	
	operands.pop();

	switch (token.m_BinaryOperator) {
		case EBinaryOp::Minus:
			operands.push(leftOperand - rightOperand);
			break;
		case EBinaryOp::Plus:
			operands.push(leftOperand + rightOperand);
			break;
		case EBinaryOp::Multi:
			operands.push(leftOperand * rightOperand);
			break;
		case EBinaryOp::Divis:
			operands.push(leftOperand / rightOperand);
			break;
		case EBinaryOp::Exp:
			operands.push(pow(leftOperand, rightOperand));
			break;
		default:
			return;
	}
}

void CTokenExpEvaluator::CalculateUnaryOperation(std::stack<double> & operands, const CToken & token) const {
	double operand = operands.top();
	operands.pop();

	switch (token.m_UnaryOperator) {
		case EUnaryOp::Minus:
			operands.push(operand * - 1);
			break;
		case EUnaryOp::Plus:
			operands.push(operand);
			break;
		case EUnaryOp::SFunction:
			switch (token.m_FunSimple) {
				case EFunSimple::Sin:
					operands.push(sin(operand));
					break;
				case EFunSimple::Ln:
					operands.push(log(operand));
					break;
				case EFunSimple::Log:
					operands.push(log10(operand));
					break;
				case EFunSimple::Abs:
					operands.push(abs(operand));
					break;
				case EFunSimple::Cos:
					operands.push(cos(operand));
					break;
				case EFunSimple::Ceil:
					operands.push(ceil(operand));
					break;
				case EFunSimple::Floor:
					operands.push(floor(operand));
					break;
				default:
					break;
			} // SFunction operator switch
		default:
			return;
	} // Unary operator switch
}
