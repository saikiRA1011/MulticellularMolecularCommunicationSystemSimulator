/**
 * @file VariableRatioCellList.hpp
 * @author Takanori Saiki
 * @brief
 * @version 0.1
 * @date 2022-06-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Cell.hpp"
#include "SegmentTree.hpp"
#include "SimulationSettings.hpp"
#include <stack>
#include <vector>

class VariableRatioCellList
{
    private:
    SegmentTree<int, int> sgTree;
    std::stack<Cell*> stackCells;

    public:
    VariableRatioCellList(/* args */);
    ~VariableRatioCellList();

    std::vector<Cell*> aroundCellList(const Cell&);
    void setCell(Cell*);
    void deleteCells();
};
