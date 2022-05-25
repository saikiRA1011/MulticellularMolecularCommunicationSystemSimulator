/**
 * @file Simulation.hpp
 * @author Takanori Saiki
 * @brief Simulation class
 * @version 0.1
 * @date 2022-04-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "Cell.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <numbers>
#include <omp.h>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

/**
 * @class Simulation
 * @brief Simulationの状態を管理するクラス。
 *
 */
class Simulation
{
    template<typename T>
    using FieldLine = std::vector<T>; //!< 2次元フィールドの行
    template<typename T>
    using Field = std::vector<FieldLine<T>>; //!< 2次元フィールド

    private:
    /**
     * @brief
     * シミュレーションで行うステップの絶対数。シミュレーションの時間はDELTA_TIME*SIM_STEP[単位時間]となる。
     */
    static constexpr int32_t SIM_STEP = 500;
    static constexpr int32_t CELL_NUM = 5000; //!< シミュレーションで生成するCell数

    static constexpr int32_t FIELD_X_LEN = 1024; //!< フィールドのx方向の辺の長さ。長さは2のn乗とする。
    static constexpr int32_t FIELD_Y_LEN = 1024; //!< フィールドのy方向の辺の長さ。長さは2のn乗とする。

    static constexpr int32_t GRID_SIZE_MAGNIFICATION = 16; //!< フィールドのグリッドサイズの倍率。最小は1、値は2^nである必要がある。
    static constexpr int32_t SEARCH_RADIUS           = 50; //!< この半径内にあるcellを力の計算の対象とする。

    static constexpr double DELTA_TIME = 0.1; //!< 時間スケール(1が通常時)

    // random
    std::mt19937 rand_gen{ 10 };                     //!< 乱数生成器(seedはとりあえず0)
    std::uniform_real_distribution<> randomCellPosX; //!< Cellのx座標の生成器
    std::uniform_real_distribution<> randomCellPosY; //!< Cellのy座標の生成器
    std::uniform_real_distribution<> randomForce;    //!< Cellのx,y方向の速度の生成器(ランダムウォークモデルで使用)

    std::queue<int> cellPool; //!< CellのIDを管理するためのキュー
    std::vector<Cell> cells;  //!< シミュレーションで使うCellのリスト。

    Field<std::vector<int32_t>> cellsInGrid; //!< グリッド内にcellのリスト(ID)を入れる。

    std::streambuf* consoleStream; //!< 標準出力のストリームバッファ
    std::ofstream outputfile;      //!< 標準出力の変更先ファイル

    void printHeader() const noexcept;
    void printCells(int32_t) const;

    //  std::vector<std::unordered_set<int32_t>> aroundCellSetList;
    //  周辺のCellのIDを格納する。ただし、vectorは一列分のみしか確保しない。

    std::vector<int32_t> aroundCellList(const Cell&) const;
    void resetGrid() noexcept;

    public:
    Simulation(/* args */);
    ~Simulation();

    void initCells() noexcept;

    Vec3 calcCellCellForce(Cell&) const noexcept;
    Vec3 calcRemoteForce(Cell&) const noexcept;
    Vec3 calcVolumeExclusion(Cell&) const noexcept;
    Vec3 calcForce(Cell&) const noexcept;

    int32_t nextStep() noexcept;
    int32_t run();

    // pythonにパラメタを渡す都合上必要になった。
    int32_t getFieldLen();
};

#endif // SIMULATION_H

// 例えば、(-8/2, 8/2)でランダムな値を生成すると長さ8の配列に収まるようになる。
// {[-4, -3), [-3, -2), [-2, -1), [-1, 0), [0, 1), [1, 2), [2, 3), [3, 4)}
/**
 * @brief 基本となるコンストラクタ。
 * @details
 * 各乱数生成器と遠隔力フィールドを初期化する。また、標準出力のストリームバッファを保存しておく。
 * @note
 * 例えば、(-8/2, 8/2)でランダムな値を生成すると長さ8の配列に収まるようになる。
 * {[-4, -3), [-3, -2), [-2, -1), [-1, 0), [0, 1), [1, 2), [2, 3), [3, 4)}
 */
Simulation::Simulation()
  : randomCellPosX(-FIELD_X_LEN / 2, FIELD_X_LEN / 2)
  , randomCellPosY(-FIELD_Y_LEN / 2, FIELD_Y_LEN / 2)
  , randomForce(-10, 10)
// , aroundCellSetList(FIELD_Y_LEN, std::unordered_set<int32_t>())
{
    consoleStream = std::cout.rdbuf();
}

/**
 * @brief いまのところ何もしない。
 *
 */
Simulation::~Simulation()
{}

/**
 * @brief 各セルをランダムな座標で初期化する。
 *
 */
void Simulation::initCells() noexcept
{
    for (int32_t i = 0; i < CELL_NUM; i++) {
        Cell c(i, randomCellPosX(rand_gen), randomCellPosY(rand_gen));
        cells.push_back(c);
    }

    constexpr int32_t CELL_GRID_LEN_X = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t CELL_GRID_LEN_Y = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    cellsInGrid.resize(CELL_GRID_LEN_Y);
    for (int32_t y = 0; y < CELL_GRID_LEN_Y; y++) {
        cellsInGrid[y].resize(CELL_GRID_LEN_X);
        for (int32_t x = 0; x < CELL_GRID_LEN_X; x++) {
            cellsInGrid[y][x] = std::vector<int>();
        }
    }
}

/**
 * @brief ファイルにヘッダ情報を出力する。
 *
 */
void Simulation::printHeader() const noexcept
{
    std::cout << "ID\tX\tY\tZ\tVx\tVy\tVz\tN_contact\tContact_IDs" << std::endl;
}

/**
 * @brief ファイルに現在のすべてのCell情報を出力する。
 *
 * @param time
 */
void Simulation::printCells(int32_t time) const
{
    std::ostringstream sout;
    sout << std::setfill('0') << std::setw(5) << time;

    std::string outputPath = "./result/cells_" + sout.str();
    std::ofstream ofs(outputPath);
    std::cout.rdbuf(ofs.rdbuf()); // 標準出力の出力先を指定ファイルに変更

    printHeader();
    for (int32_t i = 0; i < (int32_t)cells.size(); i++) {
        cells[i].printCell();
    }

    std::cout.rdbuf(consoleStream);
}

/**
 * @brief 指定したCellの周囲にあるCellのIDリストを返す。
 *
 * @param c
 * @return std::vector<int>
 * @note CHECK_WIDTHはcalcRemoteForceのLAMBDAより大きくするのが理想。
 */
std::vector<int> Simulation::aroundCellList(const Cell& c) const
{
    std::vector<int> aroundCells;
    constexpr int32_t CHECK_GRID_WIDTH = (SEARCH_RADIUS + GRID_SIZE_MAGNIFICATION - 1) / GRID_SIZE_MAGNIFICATION; // 切り上げの割り算

    Vec3 pos = c.getPosition();

    const int32_t scaledY = (pos.y + FIELD_Y_LEN / 2) / GRID_SIZE_MAGNIFICATION;
    const int32_t scaledX = (pos.x + FIELD_X_LEN / 2) / GRID_SIZE_MAGNIFICATION;

    constexpr int32_t GRID_X_WIDTH = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t GRID_Y_WIDTH = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    for (int32_t y = scaledY - CHECK_GRID_WIDTH; y <= scaledY + CHECK_GRID_WIDTH; y++) {
        for (int32_t x = scaledX - CHECK_GRID_WIDTH; x <= scaledX + CHECK_GRID_WIDTH; x++) {
            // 範囲外の場合はスキップ
            if (x < 0 || GRID_X_WIDTH <= x || y < 0 || GRID_Y_WIDTH <= y) {
                continue;
            }

            for (int i = 0; i < (int32_t)cellsInGrid[y][x].size(); i++) {
                int32_t id = cellsInGrid[y][x][i];

                const bool isSame    = (id == c.id);
                const bool isInRange = c.getPosition().dist(cells[id].getPosition()) <= SEARCH_RADIUS;

                // 調べるセルが自分自身、あるいは距離がSEARCH_RADIUSより離れている場合はスキップ
                if (isSame || !isInRange) {
                    continue;
                }

                aroundCells.push_back(id);
            }
        }
    }

    return aroundCells;
}

void Simulation::resetGrid() noexcept
{
    constexpr int32_t GRID_X_WIDTH = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t GRID_Y_WIDTH = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    // グリッドに保存されているCellのリストを初期化する。O(n^2) nは1辺の長さ
    for (int32_t y = 0; y < GRID_Y_WIDTH; y++) {
        for (int32_t x = 0; x < GRID_X_WIDTH; x++) {
            cellsInGrid[y][x].clear();
        }
    }

    for (int32_t i = 0; i < (int32_t)cells.size(); i++) {
        Vec3 pos = cells[i].getPosition();

        const int32_t scaledY = (pos.y + FIELD_Y_LEN / 2) / GRID_SIZE_MAGNIFICATION;
        const int32_t scaledX = (pos.x + FIELD_X_LEN / 2) / GRID_SIZE_MAGNIFICATION;

        cellsInGrid[scaledY][scaledX].push_back(i);
    }
}

/**
 * @brief  与えられたCellに対して他のCellから働く力を計算する。
 *
 * @param c
 * @return Vec3
 * @details Cellから働く力は遠隔力と近隣力の2つで構成される。さらに、近接力は体積排除効果と接着力の2つに分類される。
 */
Vec3 Simulation::calcCellCellForce(Cell& c) const noexcept
{
    Vec3 force = Vec3::zero();

    std::vector<int> aroundCells = aroundCellList(c);

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        int32_t id        = aroundCells[i];
        const Vec3 diff   = c.getPosition() - cells[id].getPosition();
        const double dist = diff.length();

        constexpr double LAMBDA      = 30.0;
        constexpr double COEFFICIENT = 0.5;

        // d = |C1 - C2|
        // F += c (C1 - C2) / d * e^(-d/λ)
        force += -diff.normalize().timesScalar(COEFFICIENT).timesScalar(std::exp(-dist / LAMBDA));
    }

    force = force.normalize();

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        int32_t id        = aroundCells[i];
        const Vec3 diff   = c.getPosition() - cells[id].getPosition();
        const double dist = diff.length();

        constexpr double ELIMINATION_BIAS = 10.0;
        constexpr double ADHESION_BIAS    = 0.4;
        const double sumRadius            = c.radius + cells[id].radius;
        const double overlapDist          = c.radius + cells[id].radius - dist;

        if (dist < sumRadius) {
            // force += diff.normalize().timesScalar(std::pow(1.8, overlapDist)).timesScalar(BIAS);
            force += diff.normalize().timesScalar(pow(1.0 - overlapDist / sumRadius, 2)).timesScalar(ELIMINATION_BIAS);
            force -= diff.normalize().timesScalar(pow(1.0 - overlapDist / sumRadius, 2)).timesScalar(ADHESION_BIAS);
        }
    }

    force = force.timesScalar(DELTA_TIME);

    return force;
}

/**
 * @brief 与えられたCellに対して働く遠隔力を計算する。O(n^2)
 *
 * @param c
 * @return Vec3
 * @details @f{eqnarray*}{
 * F = \sum_i \frac{c(C - C_i)}{|C-C_i|}  *
 * e^{(-|C-C_i|/\lambda)}
 * @f}
 */
Vec3 Simulation::calcRemoteForce(Cell& c) const noexcept
{
    Vec3 force = Vec3::zero();

    std::vector<int> aroundCells = aroundCellList(c);

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        int32_t id = aroundCells[i];

        Vec3 diff   = c.getPosition() - cells[id].getPosition();
        double dist = diff.length();

        constexpr double LAMBDA      = 30.0;
        constexpr double COEFFICIENT = 0.5;

        // d = |C1 - C2|
        // F += c (C1 - C2) / d * e^(-d/λ)
        force += -diff.normalize().timesScalar(COEFFICIENT).timesScalar(std::exp(-dist / LAMBDA));
    }

    force = force.timesScalar(DELTA_TIME);

    return force;
}

/**
 * @brief 与えられたCellに働く体積排除効果による力を計算する。O(n^2)
 *
 * @param c
 * @return Vec3
 * @details @f{eqnarray*}{
 * F = \sum_i
 * @f}
 */
Vec3 Simulation::calcVolumeExclusion(Cell& c) const noexcept
{
    Vec3 force = Vec3::zero();

    std::vector<int> aroundCells = aroundCellList(c);

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        int32_t id = aroundCells[i];

        constexpr double BIAS = 10.0;

        const Vec3 diff          = c.getPosition() - cells[id].getPosition();
        const double dist        = diff.length();
        const double sumRadius   = c.radius + cells[id].radius;
        const double overlapDist = c.radius + cells[id].radius - dist;

        if (dist < c.radius + cells[id].radius) {
            // force += diff.normalize().timesScalar(std::pow(1.8, overlapDist)).timesScalar(BIAS);
            force += diff.normalize().timesScalar(pow(1.0 - overlapDist / sumRadius, 2)).timesScalar(BIAS);
        }
    }

    force = force.timesScalar(DELTA_TIME);

    return force;
}

/**
 * @brief 指定したCellにかかるすべての力を計算する。O(n^2)
 *
 * @param c
 * @return Vec3
 */
Vec3 Simulation::calcForce(Cell& c) const noexcept
{
    Vec3 force = Vec3::zero();

    force += calcCellCellForce(c);
    // force += calcRemoteForce(c);
    // force += calcVolumeExclusion(c);

    return force;
}

/**
 * @brief すべてのCellに力を加えた後、それぞれのCellの位置を更新する。
 *
 * @return int32_t
 * @details Cellの数が多いので、スレッドを用いて並列処理を行う。
 */
int32_t Simulation::nextStep() noexcept
{
    resetGrid();

    // threadを使うよりもopenMPを利用したほうが速い
    //#pragma omp parallel
    for (int i = 0; i < CELL_NUM; i++) {
        // std::cout << omp_get_thread_num() << std::endl;
        Vec3 force;
        force = calcForce(cells[i]);
        cells[i].addForce(force);
    }

    // std::vector<std::thread> threads;
    // for (int i = 0; i < CELL_NUM; i++) {
    //     Vec3 force;

    //     // force = calcRemoteForce(cells[i]);
    //     // cells[i].addForce(force);

    //     threads.emplace_back([&, i] {
    //         force = calcForce(cells[i]);
    //         cells[i].addForce(force);
    //     });
    // }

    // // すべてのスレッドが終了するのを待つ
    // for (auto& t : threads) {
    //     t.join();
    // }

    for (int32_t cellID = 0; cellID < CELL_NUM; cellID++) {
        cells[cellID].nextStep();
    }

    return 0;
}

/**
 * @brief シミュレーションを実行する。
 *
 * @return int32_t
 * @details Cellの位置更新と情報出力を繰り返すだけ。
 */
int32_t Simulation::run()
{
    printCells(0);

    for (int32_t step = 1; step < SIM_STEP; step++) {
        auto start = std::chrono::system_clock::now();
        nextStep();
        printCells(step);
        auto end = std::chrono::system_clock::now();

        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "step: " << step << "  " << msec << "msec" << std::endl;
    }

    return 0;
}

/**
 * @brief
 * なくてもいい。Pythonに情報を渡す都合上必要かもしれなかった。(使っていない)
 *
 * @return int32_t
 */
int32_t Simulation::getFieldLen()
{
    return FIELD_X_LEN;
}