/**
 * @file SimMain.cpp
 * @author Takanori Saiki
 * @brief main関数
 * @version 0.1
 * @date 2022-04-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Simulation.hpp"
#include "UserSimulation.hpp"

int main(void)
{
    UserSimulation sim;

    sim.exportConfig();
    sim.initCells();

    int32_t simResult = sim.run();

    return simResult;
}