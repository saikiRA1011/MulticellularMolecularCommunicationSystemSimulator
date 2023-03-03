/**
 * @file CellList.cpp
 * @author Takanori Saiki
 * @brief CellListのデータ構造を管理するクラス
 * @version 0.1
 * @date 2022-06-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CellList.hpp"

/**
 * @brief CellListの初期化を呼びだす。
 *
 */
CellList::CellList()
{
    init();
}

CellList::~CellList()
{
}

/**
 * @brief CellListのサイズを初期化する。
 *
 */
void CellList::init()
{
    constexpr int32_t CELL_GRID_LEN_X = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t CELL_GRID_LEN_Y = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    cellField.resize(CELL_GRID_LEN_Y);
    for (int32_t y = 0; y < CELL_GRID_LEN_Y; y++) {
        cellField[y].resize(CELL_GRID_LEN_X);
        for (int32_t x = 0; x < CELL_GRID_LEN_X; x++) {
            cellField[y][x] = std::vector<Cell*>();
        }
    }
}

/**
 * @brief 設定されたグリッドサイズに合わせてCellが入っているグリッドの座標(整数)を返却する。
 *
 * @param c
 * @return std::tuple<int32_t, int32_t>
 */
std::tuple<int32_t, int32_t> CellList::getGridCoordinateByCellPos(const Cell& c) const
{
    Vec3 pos = c.getPosition();

    const int32_t gridX = (pos.x + FIELD_X_LEN / 2) / GRID_SIZE_MAGNIFICATION;
    const int32_t gridY = (pos.y + FIELD_Y_LEN / 2) / GRID_SIZE_MAGNIFICATION;

    return std::forward_as_tuple(gridX, gridY);
}

/**
 * @brief 指定したCellの周囲にあるCellのIDリストを返す。
 *
 * @param c
 * @return std::vector<int>
 * @note CHECK_WIDTHはcalcRemoteForceのLAMBDAより大きくするのが理想。
 */
std::vector<Cell*> CellList::aroundCellList(const Cell& c) const
{
    std::vector<Cell*> aroundCells;
    constexpr int32_t CHECK_GRID_WIDTH = (SEARCH_RADIUS + GRID_SIZE_MAGNIFICATION - 1) / GRID_SIZE_MAGNIFICATION; // 切り上げの割り算

    auto [gridX, gridY] = getGridCoordinateByCellPos(c);

    for (int32_t y = gridY - CHECK_GRID_WIDTH; y <= gridY + CHECK_GRID_WIDTH; y++) {
        for (int32_t x = gridX - CHECK_GRID_WIDTH; x <= gridX + CHECK_GRID_WIDTH; x++) {
            std::vector<Cell*> cellInGrid = std::move(getCellInGrid(x, y));

            // グリッド内にCellが存在していない(あるいはグリッド外を参照していれば)ここで飛ばす
            if (cellInGrid.empty()) {
                continue;
            }

            for (int i = 0; i < (int32_t)cellInGrid.size(); i++) {
                if (checkInSearchRadius(&c, cellInGrid[i])) {
                    aroundCells.emplace_back(cellInGrid[i]);
                }
            }
        }
    }

    return aroundCells;
}

/**
 * @brief 指定されたグリッド座標(x, y)に含まれるCellのvectorを返す。x, yが範囲外なら空のvectorを返す。
 *
 * @param x
 * @param y
 * @return std::vector<Cell*>
 */
std::vector<Cell*> CellList::getCellInGrid(const int32_t x, const int32_t y) const
{
    constexpr int32_t GRID_X_WIDTH = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t GRID_Y_WIDTH = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;
    // 範囲外の場合は空のvectorを返す
    if (x < 0 || GRID_X_WIDTH <= x || y < 0 || GRID_Y_WIDTH <= y) {
        return std::vector<Cell*>();
    }

    return cellField[y][x];
}

/**
 * @brief Cell cの範囲内にCell dが存在するかを返す。ただし、c == dのときもfalseにする。
 *
 * @param c
 * @param d
 * @return true
 * @return false
 */
bool CellList::checkInSearchRadius(const Cell* c, const Cell* d) const
{
    const bool isSame    = (c->id == d->id);
    const bool isInRange = c->getPosition().dist(d->getPosition()) <= SEARCH_RADIUS;

    // 調べるセルが自分自身、あるいは距離がSEARCH_RADIUSより離れている場合はfalse
    if (isSame || !isInRange) {
        return false;
    }

    return true;
}

/**
 * @brief CellListに保存されているCellの配列をすべて削除する。
 *
 */
void CellList::resetGrid() noexcept
{
    constexpr int32_t GRID_X_WIDTH = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t GRID_Y_WIDTH = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    // グリッドに保存されているCellのリストを初期化する。O(n^2) nは1辺の長さ
    for (int32_t y = 0; y < GRID_Y_WIDTH; y++) {
        for (int32_t x = 0; x < GRID_X_WIDTH; x++) {
            cellField[y][x].clear();
        }
    }
}

/**
 * @brief CellListのグリッドにCellのポインタを登録する。
 *
 * @param cell
 */
void CellList::addCell(Cell* cell)
{
    Vec3 pos = cell->getPosition();

    const int32_t scaledY = (pos.y + FIELD_Y_LEN / 2) / GRID_SIZE_MAGNIFICATION;
    const int32_t scaledX = (pos.x + FIELD_X_LEN / 2) / GRID_SIZE_MAGNIFICATION;

    cellField[scaledY][scaledX].emplace_back(cell);
}