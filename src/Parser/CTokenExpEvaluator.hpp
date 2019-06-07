#pragma once

#include "CToken.hpp"

#include <vector>
#include <stack>
#include <cmath>

/** Class CTokenExpEvaluator is used for evaluation of CToken expressions */

class CTokenExpEvaluator {
 public:
	/**
	 * Calculates cell value from token expression a dependent cell values
	 * @param[in] expr Token expression
	 * @param[in] depend table of cell positions and their values
	 * @return calculated value of the cell
	 */
	double Evaluate (const std::vector<CToken> & expr, const TCell_dep_table & depend) const;
 private:
	/**
	 * Calculates value of the token operand
	 * @param[in] token operand token
	 * @param[in] depend table of cell positions and their values
	 * @return calculated value of the token
	 */
	double getOperandValue(const CToken & token, const TCell_dep_table & depend) const;
	/**
	 * Calculates operation
	 * @param[in] operands stack of operand values
	 * @param[in] token operator token
	 */
	void CalculateOperation(std::stack<double> & operands, const CToken & token) const;
	/**
	 * Calculates value of the function token operand
	 * @param[in] token function operand token
	 * @param[in] depend table of cell positions and their values
	 * @return calculated value of the token
	 */
	double getFunctionValue(const CToken & token, const TCell_dep_table & depend) const;
	/**
	 * Calculates value of the aggregate function Max
	 * @param[in] token function operand token
	 * @param[in] depend table of cell positions and their values
	 * @return maximum value of selected cells
	 */
	double CalculateFunctionMax(const CToken & token, const TCell_dep_table & depend) const;
	/**
	 * Calculates value of the aggregate function Sum
	 * @param[in] token function operand token
	 * @param[in] depend table of cell positions and their values
	 * @return sum of selected cells
	 */
	double CalculateFunctionSum(const CToken & token, const TCell_dep_table & depend) const;
	/**
	 * Calculates value of the aggregate function Average
	 * @param[in] token function operand token
	 * @param[in] depend table of cell positions and their values
	 * @return average value of selected cells
	 */
	double CalculateFunctionAverage(const CToken & token, const TCell_dep_table & depend) const;
	/**
	 * Calculates binary operation: takes two values from the stack and puts result to the stack
	 * @param[in] operands stack of operand values
	 * @param[in] token operator token
	 */
	void CalculateBinaryOperation(std::stack<double> & operands, const CToken & token) const;
	/**
	 * Calculates unary operation: takes one value from the stack and puts result to the stack
	 * @param[in] operands stack of operand values
	 * @param[in] token operator token
	 */
	void CalculateUnaryOperation(std::stack<double> & operands, const CToken & token) const;
};
