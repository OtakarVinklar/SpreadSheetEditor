#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <stack>

#include "../Sheet/ECell.hpp"
#include "CToken.hpp"
#include "../Constants.hpp"


/** Class CTextParser is used for parsing user input */
//Could have been done better with one instance per one input
// properties would be input string and position
class CTextParser {
 public:
	/** 
	 * Parses user input
	 * @param[in] content user input, which should be parsed
	 * @param[out] cellType which cell type should be created
	 * @param[out] numberValue if expression is number, returns number value
	 * @param[out] dependencies returns dependency table
	 * @param[out] postfixExpr returns tokenized postfix expression
	 * @param[out] isNumber returns information whether the cell has number value
	 * @return whether the inputed expression is valid
	 */
	bool Parse(const std::string & content, ECell & cellType, double & numberValue, TCell_dep_table & dependencies,
						 std::vector<CToken> & postfixExpr, bool & isNumber, std::string & exception) const;
	/**
	 * Reads unsigned integer number from string
	 * @param[in] input string from which number should be read
	 * @param[out] number where the read number should be stored
	 * @param[in/out] position starting position in the input string
	 * @return whether the reading was sucessful
	 */
	bool ReadIntNumber(const std::string & input, unsigned & number, unsigned & position) const;
 private:
	/**
	 * Reads number from string
	 * @param[in] input string from which number should be read
	 * @param[out] number where the read number should be stored
	 * @param[in/out] position starting position in the input string
	 * @return whether the reading was sucessful
	 */
	bool ReadNumber(const std::string & input, double & number, unsigned & position) const;
	/**
	 * Reads Cell position from string
	 * @param[in] input string from which cell position should be read should be read
	 * @param[out] position which should be read
	 * @param[in/out] position starting position in the input string
	 * @return whether the reading was sucessful
	 */
	bool ReadCellLink(const std::string & input, TPosition & cellLink, unsigned & position) const;
	/**
	 * Converts input string infix expression to token infix expression
	 * @param[in] input string from which number should be read
	 * @param[out] tokens vector of tokens, which represents tokinezed infix expression
	 * @return whether the input expression is valid
	 */
	bool Tokenize(const std::string & input, std::vector<CToken> & tokens) const;
	/**
	 * Converts token infix expression to token postfix expression
	 * @param[in] infixTokenExpr token infix expression
	 * @param[out] postfixExpr output token postfix epression
	 * @return whether the input expression is valid
	 */
	bool ConvertToPostfix(const std::vector<CToken> & infixTokenExpr, std::vector<CToken> & postfixExpr) const;
	/**
	 * Converts alfabet column labeling to number value
	 * @param[in] label column character label
	 * @return value of column character label
	 */
	unsigned FromColumnAlfabetLabelingToNumber(std::string label) const;
	/**
	 * Reads function token from string
	 * @param[in] input string from which the function token should be read
	 * @param[out] token function which should be read and converted to token
	 * @param[in/out] position starting position in the input string
	 * @return whether the reading was sucessful
	 */
	bool ReadFunction(const std::string & input, CToken & token, unsigned & position) const;
	/**
	 * Reads cell link or value on which the function should be applied 
	 * @param[in] input string
	 * @param[out] token function token
	 * @param[in/out] position starting position in the input string
	 * @return whether the reading was sucessful
	 */
	bool ReadSimpleFunction(const std::string & input, CToken & token, unsigned & pos, EFunSimple function) const;
	/**
	 * Reads left upper and lower right cell link of table of cell links which should be applied in the function  
	 * @param[in] input string from which the cell link should be read
	 * @param[out] cellLinks left upper and lower right cell link of table of cell
	 * @param[in/out] position starting position in the input string
	 * @return whether the reading was sucessful
	 */
	bool ReadAggregateFunction(const std::string & input, std::pair<TPosition, TPosition> &  cellLinks, unsigned & pos) const;
	/**
	 * Ignores white spaces and finds position with no white space or end
	 * @param[in] input string which should be iterated
	 * @param[in/out] position in the input string
	 */
	void SkipWhiteSpace(const std::string input, unsigned & pos) const;
	/**
	 * Calculates dependency table with zero values
	 * @param[out] dependencies table with cell position and value of the cell
	 * @param[in] postfixExpr expression which stores cell links
	 */
	void CalculateDependencies(TCell_dep_table & dependencies, const std::vector<CToken> & postfixExpr) const;
	/**
	 * Adds cell dependencies from aggregate function token to dependencies table
	 * @param[in] token aggregate function token
	 * @param[out] dependencies list of cell dependencis
	 */
	void PutAggregateFunDep(const CToken & token, TCell_dep_table & dependencies) const;
	/**
	 * Validates postfix expression
	 * @param[in] postfix expression to be validated 
	 * @return whether the expression is valid
	 */
	bool isValidPostfix(const std::vector<CToken> & postfix) const;

};
