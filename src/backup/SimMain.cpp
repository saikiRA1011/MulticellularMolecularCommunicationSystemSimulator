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

#include "UserSimulation.hpp"
#include "core/Simulation.hpp"

int main(void)
{
    bool res = SimulationSettings::init_settings();

    if (!res) {
        std::cout << "Failed to initialize settings." << std::endl;
        return -1;
    }

    UserSimulation sim;

    sim.exportConfig();
    sim.initCells();

    int32_t simResult = sim.run();

    return simResult;
}