#pragma once

#include "CCellMatrix.hpp"
#include "CCell.hpp"
#include "CStringCell.hpp"
#include "CNumberCell.hpp"
#include "CEquationCell.hpp"
#include "ECell.hpp"
#include "../Constants.hpp"
#include "../Parser/CToken.hpp"
#include "../Parser/CTextParser.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <set>


/**
 * Class CSheet is used for representing SpreadSheet and its every fuctionality
 */
class CSheet {
 public:
	/**
	 * Saves currently edited spread sheet to a file
	 * @param[in] path descrtibes, where to save a file
	 * @return Returns whether the file was saved
	 */
	bool SaveToFile(const std::string & path) const;
	/**
	 * Loads data from file specified by file path
	 * @param[in] path descrtibes, where to find the file to load
	 * @return Returns whether the file was succesfully loaded
	 */
	bool LoadFile(const std::string & path);
	/** Erases all cell content */
	void EraseData();
	/** 
	 * Erases cell
	 * @param[in] pos position of cell, which should be removed
	 */
	void Erase(TPosition pos);
	/**
	 * Gets cell content (user input)
	 * @param[in] pos position of cell in the matrix
	 * @return string content which is specified by matrix position
	 */
	const std::string & getCellContent(TPosition pos) const;
	/**
	 * Set cell - initilizes cell on the basis of content and refreshes all cell values
	 * @param[in] pos position of cell in the matrix
	 * @param[in] content string which determins cell value
	 */
	void SetCell(TPosition pos, const std::string & content);
	/**
	 * Gets cell value
	 * @param[in] pos position of cell in the matrix
	 * @return string value which is specified by matrix position
	 */
	const std::string & getCellValue(TPosition pos) const;

	/**
	 * Gets cell content (user input)
	 * @param[in] pos position of cell in the matrix
	 * @return string content which is specified by matrix position
	 */
	const std::string getCellOther(TPosition pos) const; // DELETE


 private:
	/** Data structure used for storing spreadsheet cells */
	CCellMatrix m_Cells;
	/** Text parser using to parse user cell input */
	CTextParser m_Parser;
	/** Refreshes all dependent cell values - also checks cells for recursive calls */
	void RefreshCellValues();
	/**
	 * Adds cell to the cell matrix
	 * @param[in] pos position of cell in the matrix
	 * @param[in] content string which determins cell value
	 */
	void AddCell(TPosition pos, const std::string & content);
};

