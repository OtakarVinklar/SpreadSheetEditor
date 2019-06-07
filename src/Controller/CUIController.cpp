#include "CUIController.hpp"
#include <cctype>


void CUIController::PrintMenu(unsigned selection) const {
	mvprintw(0,0,"Menu");
	// left corner menu position
	unsigned leftCornerRow = MENU_POS.first;
	unsigned leftCornerCol = MENU_POS.second;
	for (unsigned i = 0; i < MENU_OPTIONS_COUNT; ++i) {
		if (selection == i) {
			attron(COLOR_PAIR(2));
			mvprintw(leftCornerRow + i, leftCornerCol, MENU_OPTIONS[i].data());
			attroff(COLOR_PAIR(2));
			continue;
		}
		mvprintw(leftCornerRow + i, leftCornerCol, MENU_OPTIONS[i].data());
	}
}

void CUIController::PrintSheetLayout(unsigned rows, unsigned columns, TPosition topLeftCellPos) const {
	TPosition pos  = SHEET_ZERO_CELL_POS;
	pos.first -= 2; --pos.second;
	// printing vertical lines with inner corners
	for (unsigned col = 0; col <= columns; ++col)
		for (unsigned row = 0; row <= rows; ++row) {
			mvwaddch(stdscr, pos.first + row*SHEET_ROW_SIZE, pos.second + col * SHEET_COL_SIZE, ACS_VLINE);
			mvwaddch(stdscr, pos.first + row*SHEET_ROW_SIZE + 1, pos.second + col * SHEET_COL_SIZE, ACS_PLUS);
		}
	// printing special bottom corner characters
	for (unsigned col = 0; col <= columns; ++col)
		mvwaddch(stdscr, pos.first + rows*SHEET_ROW_SIZE + 1, pos.second + col * SHEET_COL_SIZE, ACS_BTEE);
	// printing special right corner characters
	for (unsigned row = 0; row <= rows; ++row) {
		mvwaddch(stdscr, pos.first + row*SHEET_ROW_SIZE + 1, pos.second + columns * SHEET_COL_SIZE, ACS_RTEE);
	}
	// printing special bottom right corner character
	mvwaddch(stdscr, pos.first + rows*SHEET_ROW_SIZE + 1, pos.second + columns * SHEET_COL_SIZE, ACS_LRCORNER);

	// printing horizontal lines
	++pos.first; ++pos.second;
	for (unsigned row = 0; row <= rows; ++row)
		for (unsigned col = 0; col < columns; ++col) {
			wmove(stdscr, pos.first + row * SHEET_ROW_SIZE, pos.second + col * SHEET_COL_SIZE);
			whline(stdscr, ACS_HLINE, SHEET_COL_SIZE - 1);	// ACS_HLINE
		}
	// printing left indent lines for row numbering
	pos.second = 0;
	for (unsigned row = 0; row <= rows; ++row) {
		wmove(stdscr, pos.first + row * SHEET_ROW_SIZE, pos.second);
		whline(stdscr, ACS_HLINE, SHEET_LEFT_INDENT - 1);	// ACS_HLINE
	}
	// printing row numbering
	pos.first += 1;
	for (unsigned row = 0; row < rows; ++row) {
		wmove(stdscr, pos.first + row * SHEET_ROW_SIZE, pos.second);
		printw("%d", row + topLeftCellPos.first);
	}
	// printing column numbering
	pos.first -= 2; pos.second = SHEET_LEFT_INDENT + 1;
	for (unsigned col = 0; col < columns; ++col) {
		wmove(stdscr, pos.first, pos.second + col * SHEET_COL_SIZE);
//		printw("%d", col + topLeftCellPos.second);
		printw(NumberToColumnAlfabetLabeling(col + topLeftCellPos.second).data());
	}
	// printing shortcuts at the bottom of the screen

	unsigned col, row;
	getmaxyx(stdscr, row, col);
	attron(COLOR_PAIR(2));
	mvprintw(row - 1, SHEET_LEFT_INDENT , HINT_HELP.data());
	mvprintw(row - 1, SHEET_LEFT_INDENT + HINT_HELP.length() + 3, HINT_QUIT.data());
	attroff(COLOR_PAIR(2));
}

void CUIController::HighlightCellPos(TPosition pos, TPosition topLeftCellPos) const {
	// top left corner position 
	TPosition posTLC = SHEET_ZERO_CELL_POS;
	posTLC.first += (pos.first - topLeftCellPos.first ) * SHEET_ROW_SIZE - 1;
	posTLC.second += (pos.second - topLeftCellPos.second ) * SHEET_COL_SIZE - 1;
	// bottom right corner position
	TPosition posBRC = posTLC;
	posBRC.first += SHEET_ROW_SIZE;
	posBRC.second += SHEET_COL_SIZE;

	attron(COLOR_PAIR(1));
	PrintRectangle(posTLC, posBRC);
	attroff(COLOR_PAIR(1));
}

bool CUIController::CheckTermSize() const {
	unsigned width, height;
	getmaxyx(stdscr, height, width);
	return height > MIN_TERM_HEIGHT && width > MIN_TERM_WIDTH;
}

void CUIController::PrintSmallTerm() const {
	unsigned width, height;
	getmaxyx(stdscr, height, width);
	mvprintw(MENU_POS.first, MENU_POS.second,"This screen has %d rows and %d columns",height,width);
	mvprintw(MENU_POS.first + 1, MENU_POS.second,"Resize your terminal for at least %d rows and %d columns", MIN_TERM_HEIGHT, MIN_TERM_WIDTH);
	mvprintw(0,0,"Small terminal");
}

void CUIController::PrintHelp() const {
	mvprintw(0,0,"Help");
	mvprintw(HELP_POS.first, MENU_POS.second,"Sheet keys:");
	mvprintw(HELP_POS.first + 2, MENU_POS.second,"Enter");
	mvprintw(HELP_POS.first + 2, MENU_POS.second + TEXT_INDENT,"Edits and refreshes cell");
	mvprintw(HELP_POS.first + 3, MENU_POS.second,"'b', 'q'");
	mvprintw(HELP_POS.first + 3, MENU_POS.second + TEXT_INDENT,"Goes back to menu");
	mvprintw(HELP_POS.first + 4, MENU_POS.second,"F2");
	mvprintw(HELP_POS.first + 4, MENU_POS.second + TEXT_INDENT,"Prints this help screen");

	mvprintw(HELP_POS.first + 6, MENU_POS.second,"Fuction syntax:");

	mvprintw(HELP_POS.first + 8, MENU_POS.second,"Every function expression starst with '=' and can have following operators and function");
	mvprintw(HELP_POS.first + 9, MENU_POS.second,"To link a cell value into expression use: 'column''row' label");

	mvprintw(HELP_POS.first + 11, MENU_POS.second,"Operators:");
	mvprintw(HELP_POS.first + 13, MENU_POS.second, "-, +, /, *, ^");

	mvprintw(HELP_POS.first + 15, MENU_POS.second,"Functions:");
	mvprintw(HELP_POS.first + 17, MENU_POS.second,"Simple functions");
	mvprintw(HELP_POS.first + 17, MENU_POS.second + TEXT_INDENT, "ln(), log(), ceil(), floor(), sin(), cos()");
	mvprintw(HELP_POS.first + 18, MENU_POS.second,"Aggregate functions");
	mvprintw(HELP_POS.first + 18, MENU_POS.second + TEXT_INDENT, "avr(), sum(), max()");

	mvprintw(HELP_POS.first + 20, MENU_POS.second,"Examples:");
	mvprintw(HELP_POS.first + 22, MENU_POS.second,"= 5 * ln( A1 )");
	mvprintw(HELP_POS.first + 22, MENU_POS.second + TEXT_INDENT, "Multiplies natural logarithm of value in cell A1 by constant 5");
	mvprintw(HELP_POS.first + 23, MENU_POS.second,"= avr (B3 : C4) ^ 2");
	mvprintw(HELP_POS.first + 23, MENU_POS.second + TEXT_INDENT, "Calculates average of cells B3, B4, C3, C4 to the power of 2");
}

void CUIController::PrintAbout() const {
	mvprintw(0,0,"About");
	mvprintw(MENU_POS.first, MENU_POS.second,"Simple spreadsheet editor");
	mvprintw(MENU_POS.first + 2, MENU_POS.second,"Editor allows user to do simple yet powerful calculations");
	mvprintw(MENU_POS.first + 4, MENU_POS.second,"Was created as a school project");
}

void CUIController::PrintEnd() const {
	mvprintw(MENU_POS.first, MENU_POS.second,"Are you sure you want to quit? All unsaved changes will be lost!");
	mvprintw(MENU_POS.first + 1, MENU_POS.second,"If you would like to continue press 'b' key for back");
	mvprintw(MENU_POS.first + 2, MENU_POS.second,"otherwise press anything else");
	mvprintw(0,0,"End");
}

void CUIController::LineTextInput(std::pair<unsigned,unsigned> startPos, unsigned maxLength, std::string & str) const {
	std::string tmpString = str;
	int in;
	bool save = false;
	bool cycle = true;
  nodelay(stdscr, FALSE);
	curs_set(2);
	unsigned row = startPos.first;
	unsigned column = startPos.second;
	wmove(stdscr, row, column);
	printw(tmpString.data());
	column += tmpString.length();
	wmove(stdscr, row, column);

	while (cycle) {
		in = getch();
		switch (in) {
			case K_ENTER:
				save = true;
				cycle = false;
				continue;
			case KEY_ESCAPE:
				cycle = false;
				continue;
			case KEY_BACKSPACE: // 263
				if (!tmpString.empty()) {
//				wmove(stdscr, 0, 0); DELETE
//				printw("removing character"); DELETE
					tmpString.pop_back();
					mvwaddch(stdscr, row, --column, BLANK_CHAR);
					wmove(stdscr, row, column);
				}
				continue;
		}
		if (isprint(in) == 0)
			continue;
		if (tmpString.length() == maxLength)
			continue;
		waddch(stdscr, in);
		tmpString.push_back(in);
		++column;
	}
	if (save)
		str = tmpString;

	curs_set(0);
}

void CUIController::PrintInvalidPath() const {
	attron(COLOR_PAIR(1));
	mvprintw(0,0,"Invalid Path");
	attroff(COLOR_PAIR(1));
}

void CUIController::PrintCannotSave() const {
	attron(COLOR_PAIR(1));
	mvprintw(0,0,"Could not save file");
	attroff(COLOR_PAIR(1));
}

void CUIController::PrintCannotLoad() const {
	attron(COLOR_PAIR(1));
	mvprintw(0,0,"Could load file");
	attroff(COLOR_PAIR(1));
}


void CUIController::BoxInput(TPosition pos, unsigned maxLength, std::string & outString, const std::string & description) const {
	if (!description.empty())	{
		wmove(stdscr, pos.first, pos.second);
		printw(description.data());
		++pos.first;
	}
	PrintRectangle(pos, std::make_pair(pos.first + 2, pos.second + maxLength + 1));
	++pos.first;
	++pos.second;
	LineTextInput(pos, maxLength, outString);
}

void CUIController::BoxText(TPosition pos, unsigned boxLength, const std::string & text) const {
	PrintRectangle(pos, std::make_pair(pos.first + 2, pos.second + boxLength + 1));
	wmove(stdscr, pos.first + 1, pos.second + 1);
	printw(text.data());
}

void CUIController::PrintRectangle(TPosition leftUpp, TPosition rightBot) const {
	wmove(stdscr, leftUpp.first, leftUpp.second);
	waddch(stdscr, ACS_ULCORNER);
	whline(stdscr, ACS_HLINE, rightBot.second - leftUpp.second - 1);	
	wmove(stdscr, leftUpp.first, rightBot.second);
	waddch(stdscr, ACS_URCORNER);
	wmove(stdscr, rightBot.first, leftUpp.second);
	waddch(stdscr, ACS_LLCORNER);
	whline(stdscr, ACS_HLINE, rightBot.second - leftUpp.second - 1);
	wmove(stdscr, rightBot.first, rightBot.second);
	waddch(stdscr, ACS_LRCORNER);
	wmove(stdscr, leftUpp.first + 1, leftUpp.second);
	wvline(stdscr, ACS_VLINE, rightBot.first - leftUpp.first - 1);
	wmove(stdscr, leftUpp.first + 1, rightBot.second);
	wvline(stdscr, ACS_VLINE, rightBot.first - leftUpp.first - 1);
}

std::string CUIController::NumberToColumnAlfabetLabeling(unsigned columnNumber) const {
	std::vector<unsigned> residues;

	if (columnNumber == 0)
		return std::string("A"); 

	while (columnNumber != 0) {
		residues.push_back(columnNumber % 26);
		columnNumber /= 26;
	}
	if (residues.size() > 1)
		residues[residues.size() - 1]--;

	std::string output;
	for (int i = (residues.size() - 1); i >= 0; --i) {
		output.push_back(residues[i] + 65);
	}

	return output;
}

void CUIController::PrintCellPosition(TPosition position) const {
		mvprintw(HEADER_CELL_CONT_POS.first - 1, HEADER_CELL_CONT_POS.second, 
"Cell [%s, %d]", NumberToColumnAlfabetLabeling(position.second).data(), position.first);
}



