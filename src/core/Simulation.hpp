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

#include "../UserCell.hpp"
#include "Cell.hpp"
// #include "UserRule.hpp"
#include "../SimulationSettings.hpp"
#include "../UserMoleculeSpace.hpp"
#include "../utils/Util.hpp"
#include "CellList.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
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
    CellList cellList;                            //!< CellListのデータ構造を管理するクラス
    std::vector<std::shared_ptr<UserCell>> cells; //!< シミュレーションで使うCellのリスト。
    std::streambuf* consoleStream;                //!< 標準出力のストリームバッファ

    std::vector<std::unique_ptr<UserMoleculeSpace>> moleculeSpaces; //!< 分子の空間を管理するクラス。分子の種類ごとに1つの空間を持つ。

    // random
    std::mt19937 rand_gen{ SimulationSettings::CELL_SEED }; //!< 乱数生成器(生成器はとりあえずメルセンヌ・ツイスタ)
    std::uniform_real_distribution<> randomCellPosX;        //!< Cellのx座標の生成器
    std::uniform_real_distribution<> randomCellPosY;        //!< Cellのy座標の生成器

  private:
    Field<std::vector<std::shared_ptr<Cell>>> cellsInGrid; //!< グリッド内にcellのポインタを入れる。

    void printHeader() const noexcept;
    void printCells(int32_t time) const;
    void printMolecules(int32_t time) const;

    //  std::vector<std::unordered_set<int32_t>> aroundCellSetList;

    //  周辺のCellのIDを格納する。ただし、vectorは一列分のみしか確保しない。
    void setCellList() noexcept;

    int32_t debugCounter = 0;

    int32_t stepNumDigit;
    int32_t moleculeTypeNumDigit;

  public:
    Simulation(/* args */);
    ~Simulation();

    void exportConfig() const;

    virtual void initCells() noexcept;
    void initDirectories();

    virtual Vec3 calcCellCellForce(std::shared_ptr<UserCell>) const noexcept;
    virtual void stepPreprocess() noexcept;
    virtual void stepEndProcess() noexcept;
    Vec3 calcRemoteForce(std::shared_ptr<UserCell>, std::shared_ptr<UserCell>) const noexcept;
    Vec3 calcVolumeExclusion(std::shared_ptr<UserCell>, std::shared_ptr<UserCell>) const noexcept;
    Vec3 calcForce(std::shared_ptr<UserCell>) const noexcept;

    int32_t nextStep() noexcept;
    int32_t run();

    // pythonにパラメタを渡す都合上必要になった。
    int32_t getFieldLen();
};
