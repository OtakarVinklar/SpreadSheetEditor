#pragma once

#include "CCell.hpp"

/** Cell CStringCell represents cells with text content */

class CStringCell : public CCell {
 public:
	/** Constructor with content */
	CStringCell(const std::string & content): CCell(content), m_Value(content) {}
	CStringCell(const std::string & content, const std::string value): CCell(content), m_Value(value) {}
	/** get cell value */
	virtual const std::string & getValue() const {
		return m_Value;
	}
	/**
	 * Gets information, whether the cell value depends on other cells
	 * @return whether the cell value depends on other cells
	 */
	virtual bool isDependent() const {
		return false;
	}
	/**
	 * Gets numeric value of the cell, if it has any 
	 * @param[in] numerical value of the cell
	 * @return whether the cell has numerical value
	 */
	virtual bool getNumValue(double & value) const {
		return false;
	}
	/**
	 * Returns whether the cell has numeric value 
	 * @return whether the cell has numerical value
	 */
	virtual bool isNumValue() const {
		return false;
	}
	/**
	 * Gets information whether the cell is empty 
	 * @return whether the cell is empty
	 */
	virtual bool isEmpty() const {
		return false;
	}
 private:
	/** Value to be printed */
	std::string m_Value;
};
