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
    bondCount.resize(SimulationSettings::CELL_NUM, 0);
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
    // UserCell c1(CellType::WORKER, -10, -10, 10);
    // UserCell c2(CellType::WORKER, 10, -10, 10);
    // UserCell c3(CellType::WORKER, 0, 10, 10);
    // UserCell c4(CellType::WORKER, 0, 1, 10);
    // cells.push_back(std::make_shared<UserCell>(c1));
    // cells.push_back(std::make_shared<UserCell>(c2));
    // cells.push_back(std::make_shared<UserCell>(c3));
    // cells.push_back(std::make_shared<UserCell>(c4));
    // return;

    std::ifstream input_tsv("./input/init_cell");
    std::string str_buf;

    // tsvファイルを読み込み、2, 3, 4列目を取得する。
    std::getline(input_tsv, str_buf); // ヘッダ行を読み飛ばす
    while (std::getline(input_tsv, str_buf)) {
        std::istringstream iss(str_buf);
        std::string str;
        std::vector<std::string> str_vec;
        while (std::getline(iss, str, '\t')) {
            str_vec.push_back(str);
        }

        double x = std::stod(str_vec[2]);
        double y = std::stod(str_vec[3]);
        double z = std::stod(str_vec[4]);

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

    for (int i = 0; i < preCellCount; i++) {
        if (cells[i]->getCellType() == CellType::DEAD || cells[i]->getCellType() == CellType::NONE) {
            continue;
        }
        // cells[i]->metabolize();
    }

    // 細胞間の結合状態を更新する
    for (int i = 0; i < preCellCount; i++) {
        for (int j = 0; j < preCellCount; j++) {
            if (i == j) {
                continue;
            }

            const Vec3 diff   = cells[i]->getPosition() - cells[j]->getPosition();
            const double dist = diff.length();
            if (dist <= d_cont) {
                bondMatrix[i][j] = true;
            }

            if (bondMatrix[i][j] && dist >= d_max) {
                bondMatrix[i][j] = false;
            }
        }
    }

    for (int i = 0; i < preCellCount; i++) {
        cells[i]->clearAdhereCells();

        for (int j = 0; j < preCellCount; j++) {
            if (i == j) {
                continue;
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
                force += diff.normalize().timesScalar((d_max - dist) / d_max).timesScalar(2.0);
            } else {
                force += -diff.normalize().timesScalar(std::max(0.0, (dist - d_min) / (d_max - d_min))).timesScalar(2.0);
            }
        }

        force += -diff.normalize().timesScalar(std::exp(-dist / lambda)).timesScalar(0.05);

        if (dist < d_cont) {
            force += diff.normalize().timesScalar((d_cont - dist) / d_cont).timesScalar(10.0);
        }
    }

    force = force.timesScalar(SimulationSettings::DELTA_TIME);

    return force;
}