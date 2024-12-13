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
    const double dMax   = 150.0;
    const double dMin   = 80.0;
    const double dCont  = 20.0;
    const double lambda = 20.0;

  public:
    UserSimulation(/* args */);
    ~UserSimulation();

    void initCells() noexcept override;
};
