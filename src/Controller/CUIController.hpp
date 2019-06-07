#pragma once

#include "../Constants.hpp"

#include <ncurses.h>
#include <string>
#include <vector>
#include <cctype>

/**
 *	Class CUIController is used for printing user interface with the help of NCurses
 */
class CUIController {
	public:
	/**
	 * Method prints menu with selected choice
	 * @param[in] selection which option is the cursor on 
	 */
	void PrintMenu(unsigned selection) const;
	/**
	 * Checks terminal size
	 * @return returns false if the terminal is smaller than the minimal size
	 */
	bool CheckTermSize() const;
	/** Prints infomation to user about small terminal size */
	void PrintSmallTerm() const;
	/** Prints help screen */
	void PrintHelp() const;
	/** Prints about screen */
	void PrintAbout() const;
	/** Prints end screen */
	void PrintEnd() const;
	/**
	 * User inputs string
	 * @param[in] statPos position, where the input should be read
	 * @param[in] maxLength maximum input string length
	 * @param[out] str result string after user input
	 */
	void LineTextInput(TPosition startPos, unsigned maxLength, std::string & str) const;
	/** Prints invalid path error */
	void PrintInvalidPath() const;
	/** Prints file could not be saved error */
	void PrintCannotSave() const;
	/** Prints file could not be loaded error */
	void PrintCannotLoad() const;
	/**
	 * Prints box with description and reads user input
	 * @param[in] position of top left corner, where the first character should be printed
	 * @param[in] maxLength maximum input string length
	 * @param[out] outString result string after user input
	 * @param[in] description box title
	 */
	void BoxInput(TPosition pos, unsigned maxLength, std::string & outString, const std::string & description) const;
	/**
	 * Prints box with text size of
	 * @param[in] position of top left corner, where the first character should be printed
	 * @param[in] boxLength length of printed box
	 * @param[in] text to be printed inside the box
	 */
	void BoxText(TPosition pos, unsigned boxLength, const std::string & text) const;
	/**
	 * Prints rectangle
	 * @param[in] leftUpp left upper corner coordnites
	 * @param[in] rightbot right bottum corner coornites 
	 */
	void PrintRectangle(TPosition leftUpp, TPosition rightBot) const;
	/**
	 * Prints layout for spreadsheet editor screen
	 * @param[in] maxRows how many rows of cells should be printed
	 * @param[in] maxColumns how many columns of cells should be printed
	 */
	void PrintSheetLayout(unsigned maxRows, unsigned maxColumns, TPosition topLeftCellPos) const;
	/**
	 * Highlights cell at given position
	 * @param[in] pos position, that should be highlighted
	 * @param[in] topLeftCellPos cell that is in the current top left corner of the sheet
	 */
	void HighlightCellPos(TPosition pos, TPosition topLeftCellPos) const;
	/**
	 * Prints current cell position in the header
	 * @param[in] position of the cell
	 */
	void PrintCellPosition(TPosition position) const;
 private:
	/**
	 * Transforms ColumnNumber to Alfabet labeling
	 * @param[in] ColumnNumber number to be tranformed
	 * @return column alfabet lable
	 */
	std::string NumberToColumnAlfabetLabeling(unsigned columnNumber) const;
};
