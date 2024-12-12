/**
 * @file UserSimulation.hpp
 * @author Takanori Saiki
 * @brief シミュレーション固有の設計をここに記す。
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "core/Simulation.hpp"
#include <algorithm>
#include <cmath>
#include <set>

class UserSimulation : public Simulation
{
  private:
    Vec3 calcCellCellForce(std::shared_ptr<UserCell> c) const noexcept override;
    void stepPreprocess() noexcept override;
    void stepEndProcess() noexcept override;

    std::vector<std::vector<bool>> bondMatrix;
    std::vector<int> bondCount;
    const double d_max  = 150.0;
    const double d_min  = 80.0;
    const double d_cont = 20.0;
    const double lambda = 20.0;

  public:
    UserSimulation(/* args */);
    ~UserSimulation();

    void initCells() noexcept override;
};
