#include "VariableRatioCellList.hpp"

VariableRatioCellList::VariableRatioCellList()
  : sgTree(FIELD_X_LEN, FIELD_Y_LEN, 0)
{
}

VariableRatioCellList::~VariableRatioCellList()
{
}

std::vector<Cell*> VariableRatioCellList::aroundCellList(const Cell& c)
{
    std::vector<Cell*> aroundCells;

    std::cout << c.getPosition().x << " " << c.getPosition().y << std::endl;

    return aroundCells;
}

void VariableRatioCellList::setCell(Cell* c)
{
    int32_t nowWeight = sgTree.get(c->getPosition().x, c->getPosition().y);
    sgTree.update(c->getPosition().x, c->getPosition().y, nowWeight + 1);
}

void VariableRatioCellList::deleteCells()
{
    while (!stackCells.empty()) {
        Cell* c = stackCells.top();
        stackCells.pop();
        delete c;
    }
}