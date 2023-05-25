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

    // すべての細胞の力を初期化する(速度を0に設定)
    for (int i = 0; i < preCellCount; i++) {
        cells[i]->initForce();
    }

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
            auto c = std::make_shared<UserCell>(cells[i]->divide());

            // 分裂した場合は配列に新しいCellを上書き(あるいは追加)する。
            if (c->arrayIndex >= (int32_t)cells.size()) {
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

Vec3 UserSimulation::calcCellCellForce(std::shared_ptr<UserCell> c) const noexcept
{
    auto aroundCells = cellList.aroundCellList(c);
    Vec3 force       = Vec3::zero();

    switch (c->getCellType()) {
        case CellType::WORKER:
            for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
                if (aroundCells[i]->getCellType() == CellType::WORKER) {
                    force += Simulation::calcRemoteForce(c, aroundCells[i]);
                }
            }
            force = force.normalize();

            for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
                if (aroundCells[i]->getCellType() != CellType::NONE) {
                    force += Simulation::calcVolumeExclusion(c, aroundCells[i]);
                }
            }

            return force.timesScalar(DELTA_TIME);

        case CellType::DEAD:
            for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
                if (aroundCells[i]->getCellType() != CellType::NONE) {
                    force += Simulation::calcVolumeExclusion(c, aroundCells[i]);
                }
            }

            return force.timesScalar(DELTA_TIME);

        case CellType::NONE:
            return Vec3::zero();
        default:
            std::cerr << "CellType is Wrong: " << NAMEOF_ENUM(c->getCellType()) << std::endl;
            exit(1);
    }
}