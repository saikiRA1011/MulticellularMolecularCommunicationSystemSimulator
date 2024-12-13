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
    bondMatrix.resize(SimulationSettings::CELL_NUM, std::vector<bool>(SimulationSettings::CELL_NUM, false));
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
    std::uniform_real_distribution<double> rand_r(0, 150);
    std::uniform_real_distribution<double> rand_theta(0, 2 * M_PI);
    for (int i = 0; i < SimulationSettings::CELL_NUM; i++) {
        double r     = rand_r(rand_gen);
        double theta = rand_theta(rand_gen);

        double x = r * std::cos(theta);
        double y = r * std::sin(theta);

        UserCell c(CellType::WORKER, x, y, 10);
        cells.push_back(std::make_shared<UserCell>(c));
    }
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

    // 細胞間の結合状態を更新する
    for (int i = 0; i < preCellCount; i++) {
        cells[i]->clearAdhereCells();
        for (int j = 0; j < preCellCount; j++) {
            if (i == j) {
                continue;
            }

            const Vec3 diff   = cells[i]->getPosition() - cells[j]->getPosition();
            const double dist = diff.length();
            if (dist <= dCont) {
                bondMatrix[i][j] = true;
            }

            if (bondMatrix[i][j] && dist >= dMax) {
                bondMatrix[i][j] = false;
            }

            if (bondMatrix[i][j]) {
                cells[i]->adhere(*cells[j]);
            }
        }
    }
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
    Vec3 force = Vec3::zero();

    for (auto cell : cells) {
        if (c->id == cell->id)
            continue;

        const Vec3 diff   = c->getPosition() - cell->getPosition();
        const double dist = diff.length();

        if (bondMatrix[c->id][cell->id]) {
            if (c->adhereCells.size() <= 3) {
                force += diff.normalize().timesScalar((dMax - dist) / dMax).timesScalar(2.0);
            } else {
                force += -diff.normalize().timesScalar(std::max(0.0, (dist - dMin) / (dMax - dMin))).timesScalar(2.0);
            }
        }

        force += -diff.normalize().timesScalar(std::exp(-dist / lambda)).timesScalar(0.05);

        if (dist < dCont) {
            force += diff.normalize().timesScalar((dCont - dist) / dCont).timesScalar(10.0);
        }
    }

    force = force.timesScalar(SimulationSettings::DELTA_TIME);

    return force;
}