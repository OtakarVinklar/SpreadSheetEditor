#include "CSheet.hpp"

bool CSheet::SaveToFile(const std::string & path) const {
	std::ofstream ofile (path);
	if (!ofile.is_open())
		return false;
	std::set<TPosition> positions = m_Cells.getAllKeys();
	
	for (const auto & pos : positions) {
		ofile << pos.first << ' ' << pos.second << ' ' << getCellContent(pos) << '\n';
	}
	ofile.close();
	return true;
}

bool CSheet::LoadFile(const std::string & path) {
	std::ifstream ifile (path);
	if (!ifile.is_open())
		return false;

	std::stringstream buffer;
	buffer << ifile.rdbuf();
	std::vector<std::pair<TPosition, std::string>> cells;

	std::string line;
	while (std::getline(buffer, line)) {
		unsigned pos = 0, row, col;

		if (!m_Parser.ReadIntNumber(line, row, pos))
			return false;

		if (pos == line.length() || line[pos] != ' ')
			return false;
		++pos;

		if (pos == line.length() || !m_Parser.ReadIntNumber(line, col, pos))
			return false;

		if (pos == line.length() || line[pos] != ' ')
			return false;
		++pos;

		std::string content = line.substr(pos, line.length());
		cells.push_back(std::make_pair(std::make_pair(row, col), content));
	}
	EraseData();

	for (const auto & cell : cells)
		AddCell(cell.first, cell.second);
	RefreshCellValues();

	return true;
}

/** Erases all cell content */
void CSheet::EraseData() {
	m_Cells.Clear();
}

const std::string & CSheet::getCellContent(TPosition pos) const {
	return m_Cells(pos).getContent();
}

const std::string & CSheet::getCellValue(TPosition pos) const {
	return m_Cells(pos).getValue();
}


const std::string CSheet::getCellOther(TPosition pos) const {
	return m_Cells(pos).getOther();
}




void CSheet::SetCell(TPosition pos, const std::string & content) {

	AddCell(pos, content);
	RefreshCellValues();
}

void CSheet::AddCell(TPosition pos, const std::string & content) {

	if (content.empty()) {
		Erase(pos);
		return;
	}
	ECell cellType;
	double numberValue;
	TCell_dep_table dependencies;
	std::vector<CToken> postfixExpr;
	bool isNumber;
	std::string exception = "Invalid_Exp";

	if (!m_Parser.Parse(content, cellType, numberValue, dependencies, postfixExpr, isNumber, exception)) {
		m_Cells.set(pos.first, pos.second, new CStringCell(content, exception));
		return;
	}

	switch (cellType) {
		case ECell::String:
			m_Cells.set(pos.first, pos.second, new CStringCell(content));
			return;
		case ECell::Numeric:
			m_Cells.set(pos.first, pos.second, new CNumberCell(content, numberValue));
			return;
		case ECell::Equation:
			m_Cells.set(pos.first, pos.second, new CEquationCell(content, isNumber, postfixExpr, dependencies));
			return;
	}
}

void CSheet::Erase(TPosition pos) {
	m_Cells.Remove(pos.first, pos.second);
	RefreshCellValues();
}
// Not sure if inner implementation 
void CSheet::RefreshCellValues() {
	// set of cell, that cannot be calculated - their content is dependent on other cells
	std::set<TPosition> notCalc = m_Cells.getDependentKeys();
		
	while (!notCalc.empty()) {
		bool change = false;

		for (auto it = notCalc.begin(); it != notCalc.end();) {
			TCell_dep_table & table = m_Cells.getDependencyTable(*it);
			bool canBeCalculated = true;
			for (auto & dependency : table) {
				if (notCalc.find(dependency.first) != notCalc.end()) {
					canBeCalculated = false;
					break;
				}
				if (!m_Cells(dependency.first).getNumValue(dependency.second)) {
					m_Cells.set(*it, new CStringCell(m_Cells(*it).getContent(), "NonNumArg"));
					break;
				}
				// if something cannot be calculeted - calculated = false and break;				
			}
			if (canBeCalculated) {
				m_Cells.RefreshCell(*it);
				change = true;
				it = notCalc.erase(it);
			}
			else
				++it;
		}

		if (!change)
			break;
	}
	for (const auto & pos : notCalc)
		m_Cells.set(pos, new CStringCell(m_Cells(pos).getContent(), "Error_Recur"));
}
