#pragma once

#include "CUIController.hpp"
#include "../Constants.hpp"
#include "../Sheet/CSheet.hpp"

#include <ncurses.h>
#include <unistd.h>
#include <string>

/**
 * Class CProgramController is handeling user input 
 * class is responsable for handeling all funcionality by using necessary classes
 */
class CProgramController {
	public:
	/**
	 * Method with endles loop - main cycle of the program
   */
	void Run();
	/** Default constructor */
	CProgramController(): ui(), filePath(""), sheet() {} 
	private:
		/** object used for printing ui */
		CUIController ui;
		/** Path to currently modified file */
		std::string filePath;
		/**	object for sheet management */
		CSheet sheet;
		/**
		 * Checks whether the path is a valid file path	
		 * @param[in] path to be checked for validity 
		*/
		bool isValidPath(const std::string & path) const;
		/** Prepares application for an end */
		void PrepareForEnd() const;
		/** Main menu screen */
		void RunMenu();
		/** Procedure for editing current file */
		void EditSheet();
		/** Procedure for creating new file and editing (unsaved changes in current file will not be saved) */
		void NewSheet();
		/** Procedure for loading existing file (unsaved changes in current file will not be saved) */
		void LoadSheet();
		/** Saves file changes to file */
		void SaveSheet();
		/** 
		 * Saves file changes to different file if saving was succesful
		 * Otherwise no change is made
		 */
		void SaveAsSheet();
		/** Prints help screen */
		void HelpScreen() const;
		/** Prints about screen */
		void AboutScreen() const;
		/**
		 * Prints end screen and asks if user really wants to quit (all unsaved changes will be lost) 
		 * @return Returns information whether user wants to end
		 */
		bool EndScreen() const;
		/** Gives user ability to change filePath */
		void EditPath();
		/**
		 * Prints sheet values to the screen, to their assigned place
		 * @param[in] topLeftCellPos which cell should be in to left cell
		 * @param[in] rows how much rows of cells is displayed
		 * @param[in] cols how much columns of cells is displayed
		 */
		void PrintSheetValues(TPosition topLeftCellPos, unsigned rows, unsigned cols) const;
		/**
		 * Prints content value of the current cell in the header
		 * @param[in] cell which content should be displayed in the header 
		 */
		void PrintHeaderValue(TPosition cell) const;
		/**
		 * Handles resizing of the window
		 * @param[out] rows number of rows to be displayed
		 * @param[out] cols number of columns to be displayed
		 * @param[out] currentCellPos current targeting position
		 * @param[in] topLeftCell cell position which is in the top left corner
		 */
		void Resize(unsigned & rows, unsigned & cols, TPosition & currentCellPos, TPosition topLeftCell);
		/**
		 * Enables user to change value of the cell
		 * @param[in] cell content which should be changed
		 */
		void EditCellContent(TPosition cell);
};
