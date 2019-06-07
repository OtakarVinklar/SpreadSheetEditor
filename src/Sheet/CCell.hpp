#pragma once
#include <string>
#include <iostream>
#include <map>
#include "../Constants.hpp"

/** Class CCell is used as a representation of a single cell in the spread sheet table */
class CCell {
 public:
	/** Default constructor */
	CCell() {}
	virtual ~CCell() {}
	/** Constructor with content */
	CCell(const std::string & content): m_Content(content) {}
	/** operator = */
	CCell & operator = (const CCell & cell) {
		m_Content = cell.getContent();
		return *this;
	}
	bool operator == (const CCell & cell) const {
		return m_Content == cell.m_Content;
	}
	/** get cell content */
	const std::string & getContent() const {
		return m_Content;
	}
	/** get cell value - string represantation of the cell */
	virtual const std::string & getValue() const {
		return m_Content;
	}
	/** get cell other content - string represantation of the cell */
	virtual const std::string getOther() const { // DELETE
		return m_Content;
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
		return m_Content.empty();
	}
	/**
	 * Gets information, whether the cell value depends on other cells
	 * @return whether the cell value depends on other cells
	 */
	virtual bool isDependent() const {
		return false;
	}
	/**
	 * Gets cell dependecy table - cells on which the current value cell depends
	 * @return dependency table - map<posion, cellvalue>
	 */
	virtual TCell_dep_table & getDependencies() {
		return m_EmptyDependencies;
	}
	/** Recalculates cell values */
	virtual void CalculateCell() {}
 protected:
	/** content of the cell - user input */
	std::string m_Content;
	/** table on which the cell value depends */
	static TCell_dep_table m_EmptyDependencies;
};

