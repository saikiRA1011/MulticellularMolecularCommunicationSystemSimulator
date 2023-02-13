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
#include "../SimulationSettings.hpp"
#include "../utils/Util.hpp"
#include "CellList.hpp"
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
    protected:
    CellList cellList;             //!< CellListのデータ構造を管理するクラス
    std::vector<Cell> cells;       //!< シミュレーションで使うCellのリスト。
    std::streambuf* consoleStream; //!< 標準出力のストリームバッファ

    private:
    // random
    std::mt19937 rand_gen{ CELL_SEED };              //!< 乱数生成器(生成器はとりあえずメルセンヌ・ツイスタ)
    std::uniform_real_distribution<> randomCellPosX; //!< Cellのx座標の生成器
    std::uniform_real_distribution<> randomCellPosY; //!< Cellのy座標の生成器

    std::queue<int> cellPool; //!< CellのIDを管理するためのキュー

    Field<std::vector<Cell*>> cellsInGrid; //!< グリッド内にcellのポインタを入れる。

    void printHeader() const noexcept;
    void printCells(int32_t) const;

    //  std::vector<std::unordered_set<int32_t>> aroundCellSetList;

    //  周辺のCellのIDを格納する。ただし、vectorは一列分のみしか確保しない。
    void setCellList() noexcept;

    public:
    Simulation(/* args */);
    ~Simulation();

    void exportConfig() const;

    virtual void initCells() noexcept;

    virtual Vec3 calcCellCellForce(Cell&) const noexcept;
    virtual void stepPreprocess() noexcept;
    virtual void stepEndProcess() noexcept;
    Vec3 calcRemoteForce(Cell&) const noexcept;
    Vec3 calcVolumeExclusion(Cell&) const noexcept;
    Vec3 calcForce(Cell&) const noexcept;

    int32_t nextStep() noexcept;
    int32_t run();

    int32_t getNewCellIndex() noexcept;

    // pythonにパラメタを渡す都合上必要になった。
    int32_t getFieldLen();
};
