#pragma once

#include <string>
#include <map>
#include <vector>
/** position data type used for storing position information */
typedef std::pair<unsigned, unsigned> TPosition;

/** 
 * dependecy table is collection data type used for storing cell dependencies 
 * (on which cell the cell value depends)
 */
typedef std::map<TPosition, double> TCell_dep_table;

/**
 * Minimal terminal size is calculated so, that 10 rows and 6 columns fit in the window
 * MIN_TERM_HEIGHT = 6 + 10 * 2 + 2
 * MIN_TERM_WIDTH = 6 + 12 * 6
 */

const unsigned MIN_TERM_HEIGHT			= 28;
const unsigned MIN_TERM_WIDTH				= 78;


const std::vector<std::string> MENU_OPTIONS = {"Edit file", "New file", "Load file", "Save file", "Save as file", "Help", "About", "End spreadsheet"};
const unsigned MENU_OPTIONS_COUNT		= MENU_OPTIONS.size();

const unsigned MENU_OPTION_EDIT 		= 0;
const unsigned MENU_OPTION_NEW 			= 1;
const unsigned MENU_OPTION_LOAD 		= 2;
const unsigned MENU_OPTION_SAVE 		= 3;
const unsigned MENU_OPTION_SAVE_AS	= 4;
const unsigned MENU_OPTION_HELP			= 5;
const unsigned MENU_OPTION_ABOUT		= 6;
const unsigned MENU_OPTION_END			= 7;

const unsigned K_ENTER 							= 10;
const unsigned KEY_ESCAPE 					= 27;
const unsigned BLANK_CHAR 					= ' ';

// upper left corner position
const TPosition HEADER_CELL_CONT_POS = std::make_pair(3, 7);
// upper left corner position
const TPosition MENU_POS = std::make_pair(4, 4);
// upper left corner position
const TPosition HELP_POS = std::make_pair(2, 4);
const unsigned SHEET_HEADER_HEIGHT	= 9;
const unsigned SHEET_TOP_NUMBER_SIZE= 3;
const unsigned SHEET_LEFT_INDENT		= 6;
const unsigned SHEET_RIGHT_INDENT		= 1;
const unsigned SHEET_BOT_INDENT			= 2;

const unsigned SHEET_ROW_SIZE				= 2;
const unsigned SHEET_COL_SIZE				= 12;
const unsigned SHEET_CELL_MAX_CAP		= SHEET_COL_SIZE - 1;
const unsigned MAX_LEN_USR_INP			= 50;
const unsigned TEXT_INDENT					= 25;

const std::string HINT_HELP 				= "For Help press F2";
const std::string HINT_QUIT 				= "For Menu press 'q' or 'b'";

const TPosition SHEET_ZERO_CELL_POS	= std::make_pair(SHEET_HEADER_HEIGHT , SHEET_LEFT_INDENT);

const TCell_dep_table CELL_EMPTY_DEP_TABLE = TCell_dep_table();


const std::vector<std::string> FUNCTIONS = {"ln", "log", "abs", "sin", "cos", "floor", "ceil", "avr", "max", "sum", ""};
const unsigned FUNCTION_COUNT				= FUNCTIONS.size() - 1;

const unsigned FUNCTION_LN					= 0;
const unsigned FUNCTION_LOG					= 1;
const unsigned FUNCTION_ABS					= 2;
const unsigned FUNCTION_SIN					= 3;
const unsigned FUNCTION_COS					= 4;
const unsigned FUNCTION_FLOOR				= 5;
const unsigned FUNCTION_CEIL				= 6;

const unsigned FUNCTION_AVR					= 7;
const unsigned FUNCTION_MAX					= 8;
const unsigned FUNCTION_SUM					= 9;

