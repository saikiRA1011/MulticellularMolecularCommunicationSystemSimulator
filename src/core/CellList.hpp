/**
 * @file CellList.hpp
 * @author Takanori Saiki
 * @brief CellListのデータ構造を管理するクラス
 * @version 0.1
 * @date 2022-06-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../SimulationSettings.hpp"
#include "../utils/Util.hpp"
#include "../utils/Vec3.hpp"
#include "Cell.hpp"
#include <tuple>
#include <vector>

class CellList
{
  private:
    Field<std::vector<Cell*>> cellField; //!< セルのポインタが格納されている2次元配列

    std::tuple<int32_t, int32_t> getGridCoordinateByCellPos(const Cell& c) const;

  public:
    CellList();
    ~CellList();

    void init();
    std::vector<Cell*> aroundCellList(const Cell&) const;
    std::vector<Cell*> getCellInGrid(const int32_t x, const int32_t y) const;
    bool checkInSearchRadius(const Cell* c, const Cell* d) const;
    void resetGrid() noexcept;
    void addCell(Cell*);
};
