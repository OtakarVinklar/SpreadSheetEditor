#pragma once

#include "CCell.hpp"
#include "../Parser/CToken.hpp"
#include "../Parser/CTokenExpEvaluator.hpp"

#include <sstream> //string stream
#include <iomanip>
#include <vector>

/** Class CEquationCell represents CCell with an expression */

class CEquationCell : public CCell {
 public:
	/** Constructor*/
	CEquationCell(const std::string & content, bool isNumber, const std::vector<CToken> & expression,
								TCell_dep_table dependencies );
	/** get cell value */
	virtual const std::string & getValue() const {
		return m_StringRep;
	}
	/**
	 * Gets information, whether the cell value depends on other cells
	 * @return whether the cell value depends on other cells
	 */
	virtual bool isDependent() const {
		return !m_Dependencies.empty();
	}
	virtual TCell_dep_table & getDependencies() {
		return m_Dependencies;
	}
	/**
	 * Gets numeric value of the cell, if it has any 
	 * @param[in] numerical value of the cell
	 * @return whether the cell has numerical value
	 */
	virtual bool getNumValue(double & value) const {
		if (m_IsNumber)
			value = m_NumValue;
		return m_IsNumber;
	}
	/**
	 * Returns whether the cell has numeric value 
	 * @return whether the cell has numerical value
	 */
	virtual bool isNumValue() const {
		return m_IsNumber;
	}
	/**
	 * Gets information whether the cell is empty 
	 * @return whether the cell is empty
	 */
	virtual bool isEmpty() const {
		return false;
	}
	/** Recalculates cell values */
	virtual void CalculateCell();

	/** get cell other content - string represantation of the cell */
	virtual const std::string getOther() const { // DELETE
		std::stringstream ss;
		ss << "dependencies: ";
		for (const auto & dep : m_Dependencies)
			ss << "pos " << dep.first.first << ',' << dep.first.second << " value " << dep.second << "; ";
				ss << "tokens: ";
		for (const auto & tok : m_Expression)
			ss << tok << ';' ;
		return ss.str();
	}


 protected:
	/** numerical value of CNumberCell */
	double m_NumValue;
	/** string representation of CNumberCell */
	std::string m_StringRep;
	/** information whether the is has number value */
	bool m_IsNumber;
	/** Tokenized expression in Postfix format */
	std::vector<CToken> m_Expression;
	/** Cell dependencies */
	TCell_dep_table m_Dependencies;
	/** Tokenized expression in postfix evaluator */
	CTokenExpEvaluator m_Evaluator;
	/** Calculates m_NumValue with help of m_Dependencies */
	void CalculateNumValue();
};

