#pragma once

#include "Cell.hpp"
#include "SimulationSettings.hpp"
#include "Util.hpp"
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
