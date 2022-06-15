#ifndef CELL_DEFINITION_H
#define CELL_DEFINITION_H

#include "Cell.hpp"

class CellDefinition
{
    private:
    public:
    void (*volumeUpdateRule)(Cell *cell);
    void (*velocityUpdateRule)(Cell *cell);
    void (*glowRule)(Cell *cell);
    void (*divisionRule)(Cell *cell);
    void (*deathRule)(Cell *cell);
    void (*moleculeReceiveRule)(Cell *cell, int moleculeId);
    void (*moleculeEmissionRule)(Cell *cell);
};

#endif // CELL_DEFINITION_H