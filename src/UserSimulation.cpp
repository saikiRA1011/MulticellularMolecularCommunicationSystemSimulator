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

void UserSimulation::stepPreprocess() noexcept
{
    int32_t preCellCount = cells.size();

    for (int i = 0; i < preCellCount; i++) {
        if (cells[i]->getCellType() == CellType::DEAD || cells[i]->getCellType() == CellType::NONE) {
            continue;
        }

        cells[i]->metabolize();
    }
    for (int i = 0; i < preCellCount; i++) {
        if (cells[i]->getCellType() == CellType::DEAD || cells[i]->getCellType() == CellType::NONE) {
            continue;
        }
        if (cells[i]->checkWillDie()) {
            cells[i]->die();
            continue;
        }
    }

    for (int i = 0; i < preCellCount; i++) {
        if (cells[i]->getCellType() == CellType::DEAD || cells[i]->getCellType() == CellType::NONE) {
            continue;
        }

        if (cells[i]->checkWillDivide()) {
            auto c = std::make_shared<Cell>(cells[i]->divide());

            // 分裂した場合は配列に新しいCellを上書き(あるいは追加)する。
            if (c->arrayIndex >= cells.size()) {
                cells.push_back(c);
            } else {
                cells[c->arrayIndex] = c;
            }
        }
    }
}

void UserSimulation::stepEndProcess() noexcept
{
}

Vec3 UserSimulation::calcCellCellForce(std::shared_ptr<Cell> c) const noexcept
{
    // Vec3 force = Vec3::zero();

    // std::vector<Cell> aroundCells = cells;

    // constexpr double COEFFICIENT = 1.0;

    // for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
    //     Cell& cell        = aroundCells[i];
    //     const Vec3 diff   = c.getPosition() - cell.getPosition();
    //     const double dist = diff.length();

    //     constexpr double LAMBDA = 30.0;

    //     const double weight = cell.getWeight() * c.getWeight();

    //     // d = |C1 - C2|
    //     // F += c (C1 - C2) / d * e^(-d/λ)
    //     force += -diff.normalize().timesScalar(weight).timesScalar(COEFFICIENT).timesScalar(std::exp(-dist / LAMBDA));
    // }

    // force = force.normalize().timesScalar(COEFFICIENT);

    // for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
    //     Cell& cell        = aroundCells[i];
    //     const Vec3 diff   = c.getPosition() - cell.getPosition();
    //     const double dist = diff.length();

    //     constexpr double ELIMINATION_BIAS = 10.0;
    //     constexpr double ADHESION_BIAS    = 0.4;
    //     const double sumRadius            = c.radius + cell.radius;
    //     const double overlapDist          = c.radius + cell.radius - dist;

    //     if (dist < sumRadius) {
    //         // force += diff.normalize().timesScalar(std::pow(1.8, overlapDist)).timesScalar(BIAS);
    //         force += diff.normalize().timesScalar(pow(1.0 - dist / sumRadius, 2)).timesScalar(ELIMINATION_BIAS);
    //         force -= diff.normalize().timesScalar(pow(1.0 - dist / sumRadius, 2)).timesScalar(ADHESION_BIAS);
    //     }
    // }

    // force = force.timesScalar(DELTA_TIME);

    // return force;

    switch (c->getCellType()) {
        case CellType::WORKER:
            return Simulation::calcCellCellForce(c);

        case CellType::DEAD:
            return Simulation::calcVolumeExclusion(c);

        case CellType::NONE:
            return Vec3::zero();
        default:
            break;
    }
}