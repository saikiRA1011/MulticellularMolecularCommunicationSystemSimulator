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
#include <memory>
#include <tuple>
#include <vector>

class CellList
{
  private:
    Field<std::vector<std::shared_ptr<Cell>>> cellField; //!< セルのポインタが格納されている2次元配列

    std::tuple<int32_t, int32_t> getGridCoordinateByCellPos(const std::shared_ptr<Cell> c) const;

  public:
    CellList();
    ~CellList();

    void init();
    std::vector<std::shared_ptr<Cell>> aroundCellList(const std::shared_ptr<Cell> c) const;
    std::vector<std::shared_ptr<Cell>> getCellInGrid(const int32_t x, const int32_t y) const;
    bool checkInSearchRadius(const std::shared_ptr<Cell> c, const std::shared_ptr<Cell> d) const;
    void resetGrid() noexcept;
    void addCell(std::shared_ptr<Cell> cell);
};
