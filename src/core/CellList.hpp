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
#include <vector>

class CellList
{
    private:
    Field<std::vector<Cell*>> cellsInGrid; //!< セルのポインタが格納されている2次元配列

    public:
    CellList();
    ~CellList();

    void init();
    std::vector<Cell*> aroundCellList(const Cell&) const;
    void resetGrid() noexcept;
    void addCell(Cell*);
};
