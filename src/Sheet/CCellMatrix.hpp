#pragma once
 
#include "CCell.hpp"
#include "../Constants.hpp"

#include <map>
#include <memory>
#include <vector>
#include <set>

typedef std::unique_ptr<CCell> ptr_cell;

/**
 * Class CCell is used to store Cells in an infinatly large matrix
 * Class stores information, about not zero cells
 * Every cell, which is not present is returned as default element value
 */
class CCellMatrix {
 private:
	/** Container for storing pointers to cells */
	std::map<TPosition, ptr_cell> m_Cells;
	/** Zero cell - default cell, empty cell */
	static const CCell m_ZeroCell;
 public:
	/** 
	 * Method used to retrieve const value from matrix at position x, y
	 * @param[in] x row identificator starting at 0
	 * @param[in] y column identificator starting at 0
	 * @return Returns constat reference to the element at position x, y
	 */
	const CCell & operator () (unsigned x, unsigned y) const;
	/** 
	 * Method used to retrieve const value from matrix at position pos
	 * @param[in] pos position identifier
	 * @return Returns constat reference to the element at position pos
	 */
	const CCell & operator () (TPosition pos) const;
	/**
	 * Method used to retrieve Dependency table of cell at position pos
	 * @param[in] pos position identifier
	 * @return Returns reference to the cells dependency table
	 */
	TCell_dep_table & getDependencyTable (TPosition pos);
	/**
	 * Method used to calculate cell
	 * @param[in] pos position identifier
	 */
	void RefreshCell (TPosition pos);
	/** 
	 * Method used to set value at position x, y
	 * @param[in] x row identificator starting at 0
	 * @param[in] y column identificator starting at 0
	 */
	void set (unsigned x, unsigned y, CCell * element);
	/** 
	 * Method used to set value at position x, y
	 * @param[in] pos position identificator starting at [0, 0] [row, column]
	 * @param[in] element pointer to the created cell
	 */
	void set (TPosition pos, CCell * element);
	/** 
	 * Method used to remove cell at position x, y
	 * @param[in] x row identificator starting at 0
	 * @param[in] y column identificator starting at 0
	 */
	void Remove (unsigned x, unsigned y);
	/** Erases all cells */
	void Clear ();
	/** 
	 * Method used to retrieve all non zero cells positions x, y
	 * @return Returns set<position> of all positions with non-zero value
	 */
	std::set<TPosition> getAllKeys() const;
	/** 
	 * Method used to retrieve all non zero dependent cells positions x, y
	 * @return Returns set<position>
	 */
	std::set<TPosition> getDependentKeys() const;
	/** Recalculate all cells */
	void RefreshCells();

};
