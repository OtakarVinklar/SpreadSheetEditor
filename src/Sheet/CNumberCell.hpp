#pragma once

#include <sstream> //string stream
#include <iomanip>

#include "CCell.hpp"
#include "../Constants.hpp"

/** Class CNumberCell represents CCell with constant numeric value */

class CNumberCell : public CCell {
 public:
	/** Constructor with content */
	CNumberCell(const std::string & content, double number);
	/** get cell value */
	virtual const std::string & getValue() const {
		return m_StringRep;
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
		value = m_NumValue;
		return true;
	}
	/**
	 * Returns whether the cell has numeric value 
	 * @return whether the cell has numerical value
	 */
	virtual bool isNumValue() const {
		return true;
	}
	/**
	 * Gets information whether the cell is empty 
	 * @return whether the cell is empty
	 */
	virtual bool isEmpty() const {
		return false;
	}
 protected:
	/** numerical value of CNumberCell */
	double m_NumValue;
	/** string representation of CNumberCell */
	std::string m_StringRep;
};
