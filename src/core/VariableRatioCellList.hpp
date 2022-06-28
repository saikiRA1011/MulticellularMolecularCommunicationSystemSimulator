/**
 * @file VariableRatioCellList.hpp
 * @author Takanori Saiki
 * @brief SegmentTreeを用いた放射状の区間計算を行うデータ構造を管理するためのクラス。
 * @version 0.1
 * @date 2022-06-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../SimulationSettings.hpp"
#include "Cell.hpp"
#include "SegmentTree.hpp"
#include <stack>
#include <vector>

class VariableRatioCellList
{
    private:
    SegmentTree<int, int> sgTree;
    std::stack<Cell*> stackCells; //!< aroundCellListで生成した仮想セルを管理するスタック

    public:
    VariableRatioCellList(/* args */);
    ~VariableRatioCellList();

    std::vector<Cell*> aroundCellList(const Cell&);
    void setCell(Cell*);
    void deleteCells();
};
