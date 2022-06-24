/**
 * @file VariableRatioCellList.cpp
 * @author Takanori Saiki
 * @brief SegmentTreeを用いた放射状の区間計算を行うデータ構造を管理するためのクラス。
 * @version 0.1
 * @date 2022-06-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "VariableRatioCellList.hpp"

/**
 * @brief SegmentTreeを初期化する。
 *
 */
VariableRatioCellList::VariableRatioCellList()
  : sgTree(FIELD_X_LEN, FIELD_Y_LEN, 0)
{
}

VariableRatioCellList::~VariableRatioCellList()
{
}

/**
 * @brief 対象のセルを中心に放射状に広がるグリッドを考え、各グリッド内にあるセルをひとまとめにして返す。
 *
 * @param c
 * @return std::vector<Cell*>
 */
std::vector<Cell*> VariableRatioCellList::aroundCellList(const Cell& c)
{
    std::vector<Cell*> aroundCells;

    std::cout << c.getPosition().x << " " << c.getPosition().y << std::endl;

    return aroundCells;
}

/**
 * @brief 最小グリッド内にセルを追加する(実際には、そのグリッドの重さを加算するだけ)
 *
 * @param c
 */
void VariableRatioCellList::setCell(Cell* c)
{
    int32_t nowWeight = sgTree.get(c->getPosition().x, c->getPosition().y);
    sgTree.update(c->getPosition().x, c->getPosition().y, nowWeight + c->getWeight());
}

/**
 * @brief aroundCellListで取得したセルを削除する(メモリを開放する)
 *
 */
void VariableRatioCellList::deleteCells()
{
    while (!stackCells.empty()) {
        Cell* c = stackCells.top();
        stackCells.pop();
        delete c;
    }
}