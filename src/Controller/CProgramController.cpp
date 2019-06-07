#include "CProgramController.hpp"

#include <iostream>

void CProgramController::Run() {
	initscr();
	start_color();
	cbreak();
	noecho();
  keypad(stdscr, TRUE);
	curs_set(0);
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);

	if (!ui.CheckTermSize()) {
		ui.PrintSmallTerm();
		refresh();
		getch();
	}

	RunMenu();
	PrepareForEnd();
}

bool CProgramController::isValidPath(const std::string & path) const {
	if (path.empty())
		return false;
	return true;
}

void CProgramController::PrepareForEnd() const {
	endwin();
}

void CProgramController::RunMenu() {
	unsigned selection = 0;
	// input character
	int input;
	bool end = false;

	while (!end) {
		werase(stdscr);
		ui.PrintMenu(selection);
		refresh();
		input = getch();

		switch (input) {
			case KEY_UP:
				if (selection == 0)
					selection = MENU_OPTIONS_COUNT;
				--selection;
				continue;
			case KEY_DOWN:
				if (++selection == MENU_OPTIONS_COUNT)
					selection = 0;
				continue;
		}

		if (input == K_ENTER) {
			switch(selection) {
				case MENU_OPTION_EDIT:		EditSheet();		continue;
				case MENU_OPTION_NEW:			NewSheet();			continue;
				case MENU_OPTION_LOAD:		LoadSheet();		continue;
				case MENU_OPTION_SAVE:		SaveSheet();		continue;
				case MENU_OPTION_SAVE_AS:	SaveAsSheet();	continue;
				case MENU_OPTION_HELP:		HelpScreen();		continue;
				case MENU_OPTION_ABOUT:		AboutScreen();	continue;
				case MENU_OPTION_END:			end = EndScreen();
																							 		continue;
			}
		}
	}
}

/** Procedure for editing current file */
void CProgramController::EditSheet() {
	// current position
	TPosition cPos = std::make_pair(0, 0);
	bool end = false;
	unsigned width, height;
	getmaxyx(stdscr, height, width);
	// reflects terminal size
	unsigned rows = (height - SHEET_HEADER_HEIGHT - SHEET_BOT_INDENT) / SHEET_ROW_SIZE;
	unsigned cols = (width - SHEET_LEFT_INDENT - SHEET_RIGHT_INDENT) / SHEET_COL_SIZE;
	// position of top left corner
	TPosition topLeftCellPos = cPos; 
	// input
	int input;

	while (!end) {
		werase(stdscr);
		ui.PrintSheetLayout(rows, cols, topLeftCellPos);
		PrintSheetValues(topLeftCellPos, rows, cols);
		PrintHeaderValue(cPos);
		ui.HighlightCellPos(cPos, topLeftCellPos);
		refresh();

		input = getch();
		switch (input) {
			case KEY_UP:
				if (cPos.first > 0) {
					if (topLeftCellPos.first > 0 && topLeftCellPos.first == cPos.first)
						--topLeftCellPos.first;
					--cPos.first;
				}
				continue;
			case KEY_DOWN:
				if (cPos.first == (rows - 1 + topLeftCellPos.first))
					++topLeftCellPos.first;
				++cPos.first;
				continue;
			case KEY_LEFT:
				if (cPos.second > 0) {
					if (topLeftCellPos.second > 0 && topLeftCellPos.second == cPos.second)
						--topLeftCellPos.second;
					--cPos.second;
				}
				continue;
			case KEY_RIGHT:
				if (cPos.second == (cols - 1 + topLeftCellPos.second))
					++topLeftCellPos.second;
				++cPos.second;
				continue;
			case K_ENTER:
				EditCellContent(cPos);
				continue;
			case KEY_ESCAPE:
			case 'b':
			case 'q':
				end = true;
				continue;
			case KEY_RESIZE:
				Resize(rows, cols, cPos, topLeftCellPos);
				continue;
			case KEY_F(2):
				HelpScreen();
				continue;
			case KEY_DC:
				sheet.Erase(cPos);
				continue;
		}
	}
}
/** Procedure for creating new file and editing (unsaved changes in current file will not be saved) */
void CProgramController::NewSheet() {
	filePath = "";
	sheet.EraseData();
	EditSheet();
}
/** Procedure for loading existing file (unsaved changes in current file will not be saved) */
void CProgramController::LoadSheet() {
	std::string tmpPath;

	attron(COLOR_PAIR(1));
	ui.BoxInput(std::make_pair(13, 4), MAX_LEN_USR_INP, tmpPath, "Path to file, that should be loaded:");
	attroff(COLOR_PAIR(1));

	if (!isValidPath(tmpPath)) {
		ui.PrintInvalidPath();
	  nodelay(stdscr, FALSE);
		getch();
		return;
	}

	if (!sheet.LoadFile(tmpPath)) {
		ui.PrintCannotLoad();
	  nodelay(stdscr, FALSE);
		getch();
		return;
	}

	filePath = tmpPath;
}
/** Saves file changes to file */
void CProgramController::SaveSheet() {
	if (!isValidPath(filePath)) {
		EditPath();
	}
			
	if (!isValidPath(filePath)) {
		ui.PrintInvalidPath();
	  nodelay(stdscr, FALSE);
		getch();
		return;
	}
	if (!sheet.SaveToFile(filePath)) {
		ui.PrintCannotSave();
	  nodelay(stdscr, FALSE);
		getch();
		return;
	}
}
void CProgramController::SaveAsSheet() {
	std::string tmpPath = filePath;
	attron(COLOR_PAIR(1));
	ui.BoxInput(std::make_pair(13, 4), MAX_LEN_USR_INP, tmpPath, "Input file path:");
	attroff(COLOR_PAIR(1));

	if (!isValidPath(tmpPath)) {
		ui.PrintInvalidPath();
	  nodelay(stdscr, FALSE);
		getch();
		return;
	}

	if (!sheet.SaveToFile(tmpPath)) {
		ui.PrintCannotSave();
	  nodelay(stdscr, FALSE);
		getch();
		return;
	}

	filePath = tmpPath;
}
/** Prints help screen */
void CProgramController::HelpScreen() const {
	werase(stdscr);
	ui.PrintHelp();
	refresh();
  nodelay(stdscr, FALSE);
	getch();
}
/** Prints about screen */
void CProgramController::AboutScreen() const {
	werase(stdscr);
	ui.PrintAbout();
	refresh();
  nodelay(stdscr, FALSE);
	getch();
}
/**
 * Prints end screen and asks if user really wants to quit (all unsaved changes will be lost) 
 * @return Returns information whether user wants to end
 */
bool CProgramController::EndScreen() const {
	werase(stdscr);
	ui.PrintEnd();
	refresh();
  nodelay(stdscr, FALSE);
	int input = getch();
	if (input == KEY_ESCAPE || input == 'b')
		return false;
	return true;
}
void CProgramController::EditPath() {
	attron(COLOR_PAIR(1));
	ui.BoxInput(std::make_pair(13, 4), MAX_LEN_USR_INP, filePath, "Input file path:");
	attroff(COLOR_PAIR(1));
}

void CProgramController::PrintSheetValues(TPosition topLeftCellPos, unsigned rows, unsigned cols) const {
	// value - auxilliary variable for keeping current cell value
	std::string value;
	for (unsigned row = 0; row < rows; ++row) {
		TPosition cellPos = topLeftCellPos;
		// write position
		unsigned wrPosRow = SHEET_ZERO_CELL_POS.first + row * SHEET_ROW_SIZE;
		unsigned wrPosCol = SHEET_ZERO_CELL_POS.second;
		cellPos.first += row;
		for (unsigned col = 0; col < cols; ++col) {
			value = sheet.getCellValue(cellPos);
			if (value.length() > SHEET_CELL_MAX_CAP)
				value = value.substr(0, 9) + "..";
			mvprintw(wrPosRow, wrPosCol, value.data());
			++cellPos.second;
			wrPosCol += SHEET_COL_SIZE;
		}
	}
}

void CProgramController::PrintHeaderValue(TPosition pos) const {
	ui.PrintCellPosition(pos);
	ui.BoxText(HEADER_CELL_CONT_POS, MAX_LEN_USR_INP, sheet.getCellContent(pos));
//	mvprintw(HEADER_CELL_CONT_POS.first + 3, HEADER_CELL_CONT_POS.second, sheet.getCellOther(pos).data()); // DELETE
}
void CProgramController::Resize(unsigned & rows, unsigned & cols, TPosition & currentCellPos, TPosition topLeftCell) {
	while (!ui.CheckTermSize()) {
		erase();
		ui.PrintSmallTerm();
		refresh();
		getch();
	}
	unsigned width, height;
	getmaxyx(stdscr, height, width);
	rows = (height - SHEET_HEADER_HEIGHT - SHEET_BOT_INDENT) / SHEET_ROW_SIZE;
	cols = (width - SHEET_LEFT_INDENT - SHEET_RIGHT_INDENT) / SHEET_COL_SIZE;
	if ((currentCellPos.first - topLeftCell.first) >= rows)
		currentCellPos.first = topLeftCell.first + rows - 1;
	if ((currentCellPos.second - topLeftCell.second) >= cols)
		currentCellPos.second = topLeftCell.second + cols - 1;
}


void CProgramController::EditCellContent(TPosition cell) {
	std::string content = sheet.getCellContent(cell);
	ui.BoxInput(HEADER_CELL_CONT_POS, MAX_LEN_USR_INP, content, "");

	sheet.SetCell(cell, content);
}

