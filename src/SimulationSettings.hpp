/**
 * @file SimulationSettings.hpp
 * @author Takanori Saiki
 * @brief シミュレーションのパラメータなどの設定を管理するクラス
 * @version 0.1
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

class SimulationSettings
{
  public:
    SimulationSettings();
    ~SimulationSettings();
    static bool init_settings();

    static bool USE_CELL_LIST;
    static int32_t CELL_SEED;
    static int32_t SIM_STEP;             //!< シミュレーションで行うステップの絶対数。シミュレーションの時間はDELTA_TIME*SIM_STEP[単位時間]となる。
    static int32_t OUTPUT_INTERVAL_STEP; //!< シミュレーション結果を出力するステップ間隔。

    static int32_t CELL_NUM; //!< シミュレーションで生成するCell数

    static int32_t GRID_SIZE_MAGNIFICATION; //!< CellListで使用するグリッドサイズの倍率。最小は1、値は2^nである必要がある。
    static int32_t SEARCH_RADIUS;           //!< この半径内(positionの差)にあるcellを力の計算の対象とする。

    static int32_t FIELD_X_LEN; //!< シミュレーションをおこなうフィールドのx方向の辺の長さ。長さは2のn乗とする。
    static int32_t FIELD_Y_LEN; //!< シミュレーションをおこなうフィールドのy方向の辺の長さ。長さは2のn乗とする。

    static double DELTA_TIME; //!< 時間スケール(1が通常時)
};
