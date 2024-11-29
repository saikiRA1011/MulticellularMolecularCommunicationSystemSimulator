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

/**
 * @brief コンストラクタ。何も呼び出さない。
 *
 */
UserSimulation::UserSimulation(/* args */)
{
}

/**
 * @brief デストラクタ。何もしない。
 *
 */
UserSimulation::~UserSimulation()
{
}

/**
 * @brief シミュレーションで扱う細胞の初期化。ここで細胞を生成する。生成した細胞はcellsに格納する。
 * @note 各細胞の初期状態はユーザが定義する。
 *
 */
void UserSimulation::initCells() noexcept
{
    Simulation::initCells();
}

/**
 * @brief 各ステップの前処理。
 *
 */
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
        // cells[i]->metabolize();
    }

    // for (int i = 0; i < preCellCount; i++) {
    //     if (cells[i]->getCellType() == CellType::DEAD || cells[i]->getCellType() == CellType::NONE) {
    //         continue;
    //     }

    //     if (cells[i]->checkWillDivide()) {
    //         auto c = std::make_shared<UserCell>(cells[i]->divide());

    //         // 分裂した場合は配列に新しいCellを上書き(あるいは追加)する。
    //         if (c->arrayIndex >= (int32_t)cells.size()) {
    //             cells.push_back(c);
    //         } else {
    //             cells[c->arrayIndex] = c;
    //         }
    //     }
    // }
}

/**
 * @brief 各ステップの後処理。
 *
 */
void UserSimulation::stepEndProcess() noexcept
{
}

/**
 * @brief 細胞間作用の計算。細胞の種類に応じて計算を行う。
 *
 * @param c
 * @return Vec3
 */
Vec3 UserSimulation::calcCellCellForce(std::shared_ptr<UserCell> c) const noexcept
{
    auto aroundCells            = cellList.aroundCellList(c);
    Vec3 force                  = Vec3::zero();
    const Vec3 center           = Vec3(0, 0, 0);
    const Vec3 diff_from_center = c->getPosition() - center;
    Vec3 force_cont             = Vec3::zero();

    constexpr double COEFFICIENT   = 1.0;
    constexpr double REPUlSION_C   = 0.20;
    constexpr double REPULSION_LEN = 15;
    constexpr double BONDING_LEN   = 5;

    force += -diff_from_center.normalize().timesScalar(COEFFICIENT);

    for (auto i : aroundCells) {
        auto cell = cells[i];

        if (c->id == cell->id)
            continue;

        const Vec3 diff   = c->getPosition() - cell->getPosition();
        const double dist = diff.length();

        if (dist < REPULSION_LEN) {
            force += diff.timesScalar(1.0 / dist).timesScalar(REPUlSION_C).timesScalar((REPULSION_LEN - dist) / REPULSION_LEN);
        }

        const Vec3 v = cell->getVelocity();
        if (dist < BONDING_LEN) {
            force_cont += v;
        }
    }

    force += force_cont.normalize().timesScalar(0.2);
    force = force.timesScalar(SimulationSettings::DELTA_TIME);

    return force;
}