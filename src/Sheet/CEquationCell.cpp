#include "CEquationCell.hpp"

#include <iostream>

CEquationCell::CEquationCell(const std::string & content, bool isNumber, const std::vector<CToken> & expression,
														 TCell_dep_table dependencies ):
														 CCell(content), m_IsNumber(isNumber), m_Expression(expression), m_Dependencies(dependencies)
{
	CalculateCell ();
}

void CEquationCell::CalculateCell () {
	CalculateNumValue();

	if (m_IsNumber) {
		std::stringstream ss;
		ss << std::setprecision(SHEET_CELL_MAX_CAP) << m_NumValue;
		m_StringRep = ss.str();
		return;
	}
}

void CEquationCell::CalculateNumValue () {
	if (!m_IsNumber)	{
		m_NumValue = 0;
		return;
	}
	m_NumValue = m_Evaluator.Evaluate(m_Expression, m_Dependencies);
}
