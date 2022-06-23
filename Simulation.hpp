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
#include "CellList.hpp"
#include "SimulationSettings.hpp"
#include "utils/Util.hpp"
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
    private:
    CellList cellList; //!< CellListのデータ構造を管理するクラス

    // random
    std::mt19937 rand_gen{ 10 };                     //!< 乱数生成器(seedはとりあえず0)
    std::uniform_real_distribution<> randomCellPosX; //!< Cellのx座標の生成器
    std::uniform_real_distribution<> randomCellPosY; //!< Cellのy座標の生成器

    std::queue<int> cellPool; //!< CellのIDを管理するためのキュー。今は使っていない。
    std::vector<Cell> cells;  //!< シミュレーションで使うCellのリスト。

    Field<std::vector<Cell*>> cellsInGrid; //!< グリッド内にcellのポインタを入れる。

    std::streambuf* consoleStream; //!< 標準出力のストリームバッファ

    void printHeader() const noexcept;
    void printCells(int32_t) const;

    //  std::vector<std::unordered_set<int32_t>> aroundCellSetList;
    //  周辺のCellのIDを格納する。ただし、vectorは一列分のみしか確保しない。

    void setCellList() noexcept;

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
