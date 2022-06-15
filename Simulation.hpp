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

#pragma once

#include "Cell.hpp"
// #include "UserRule.hpp"
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
    static constexpr int32_t CELL_NUM = 2000; //!< シミュレーションで生成するCell数

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

    Field<std::vector<Cell*>> cellsInGrid; //!< グリッド内にcellのリスト(ID)を入れる。

    std::streambuf* consoleStream; //!< 標準出力のストリームバッファ

    void printHeader() const noexcept;
    void printCells(int32_t) const;

    //  std::vector<std::unordered_set<int32_t>> aroundCellSetList;
    //  周辺のCellのIDを格納する。ただし、vectorは一列分のみしか確保しない。

    std::vector<Cell*> aroundCellList(const Cell&) const;
    void resetGrid() noexcept;

    public:
    Simulation(/* args */);
    ~Simulation();

    void exportConfig() const;

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
