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

constexpr bool USE_CELL_LIST           = true;
constexpr int32_t CELL_SEED            = 0;
constexpr int32_t SIM_STEP             = 200; //!< シミュレーションで行うステップの絶対数。シミュレーションの時間はDELTA_TIME*SIM_STEP[単位時間]となる。
constexpr int32_t OUTPUT_INTERVAL_STEP = 1;   //!< シミュレーション結果を出力するステップ間隔。

constexpr int32_t CELL_NUM = 2000; //!< シミュレーションで生成するCell数

constexpr int32_t GRID_SIZE_MAGNIFICATION = 16; //!< CellListで使用するグリッドサイズの倍率。最小は1、値は2^nである必要がある。
constexpr int32_t SEARCH_RADIUS           = 50; //!< この半径内(positionの差)にあるcellを力の計算の対象とする。

constexpr int32_t FIELD_X_LEN = 1024; //!< シミュレーションをおこなうフィールドのx方向の辺の長さ。長さは2のn乗とする。
constexpr int32_t FIELD_Y_LEN = 1024; //!< シミュレーションをおこなうフィールドのy方向の辺の長さ。長さは2のn乗とする。

constexpr double DELTA_TIME = 0.5; //!< 時間スケール(1が通常時)