/**
 * @file UserSimulation.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "UserSimulation.hpp"

UserSimulation::UserSimulation(/* args */)
{
}

UserSimulation::~UserSimulation()
{
}

void UserSimulation::initCells() noexcept
{
    Simulation::initCells();
}

Vec3 UserSimulation::calcCellCellForce(Cell &c) const noexcept
{
    return Simulation::calcCellCellForce(c);
}