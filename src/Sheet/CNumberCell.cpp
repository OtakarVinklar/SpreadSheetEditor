#include "CNumberCell.hpp"
// Precision does not have to be set here, is also edited when printing the cell value 
CNumberCell::CNumberCell(const std::string & content, double number ): CCell(content), m_NumValue(number) {
		std::stringstream ss;
		ss << std::setprecision(SHEET_CELL_MAX_CAP) << number;
		m_StringRep = ss.str();
}
