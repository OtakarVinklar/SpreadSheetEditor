#include "CCellMatrix.hpp"
 
const CCell & CCellMatrix::operator () (unsigned x, unsigned y) const {
	const auto it = m_Cells.find(std::make_pair(x,y));
	if (it == m_Cells.end())
		return m_ZeroCell;

	return *(it->second);
}

const CCell & CCellMatrix::operator () (TPosition pos) const {
	const auto it = m_Cells.find(pos);
	if (it == m_Cells.end())
		return m_ZeroCell;

	return *(it->second);
}

void CCellMatrix::set (unsigned x, unsigned y, CCell * cell) {
	auto it = m_Cells.find(std::make_pair(x,y));
	if (it != m_Cells.end())
		it->second.reset(cell);
	else
		m_Cells.insert(std::make_pair (std::make_pair(x,y), ptr_cell(cell)));
}

void CCellMatrix::set (TPosition pos, CCell * cell) {
	auto it = m_Cells.find(pos);
	if (it != m_Cells.end())
		it->second.reset(cell);
	else
		m_Cells.insert(std::make_pair (pos, ptr_cell(cell)));
}

TCell_dep_table & CCellMatrix::getDependencyTable (TPosition pos) {
	auto it = m_Cells.find(pos);
// throw exception? - will not happen
//	if (it == m_Cells.end())
	return it->second->getDependencies();
}

void CCellMatrix::Remove (unsigned x, unsigned y) {
	auto it = m_Cells.find(std::make_pair(x,y));
	if (it != m_Cells.end())
		m_Cells.erase(it);
}

void CCellMatrix::Clear () {
	m_Cells.clear();
}

std::set<TPosition> CCellMatrix::getAllKeys() const {
	std::set<TPosition> cellSet;
	for (const auto & cell : m_Cells)
		cellSet.insert(cell.first);

	return cellSet;
}

std::set<TPosition> CCellMatrix::getDependentKeys() const {
	std::set<TPosition> cellSet;
	for (const auto & cell : m_Cells)
		if (cell.second->isDependent())
			cellSet.insert(cell.first);

	return cellSet;
}

/* inicialization of the m_ZeroCell */
const CCell CCellMatrix::m_ZeroCell = CCell( );

void CCellMatrix::RefreshCells() {
	for (const auto & cell : m_Cells)
		cell.second->CalculateCell();
}

void CCellMatrix::RefreshCell (TPosition pos) {
	auto it = m_Cells.find(pos);
	if (it != m_Cells.end())
		it->second->CalculateCell();
}

