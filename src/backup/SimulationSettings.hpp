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

#include "thirdparty/nameof.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

enum class PositionUpdateMethod
{
    ORIGINAL,
    AB4,
    AB3,
    AB2,
    EULER,
};

class SimulationSettings
{
  public:
    SimulationSettings();
    ~SimulationSettings();
    static bool init_settings();
    static void printSettings();

    // 入力値チェックのため、u_intではなくintを使う
    // そもそもu_intを使ったところでそれほど大きな値を扱うわけではないので、intで十分
    static bool USE_CELL_LIST;
    static int32_t CELL_SEED;
    static int32_t SIM_STEP;             //!< シミュレーションで行うステップの絶対数。シミュレーションの時間はDELTA_TIME*SIM_STEP[単位時間]となる。
    static int32_t OUTPUT_INTERVAL_STEP; //!< シミュレーション結果を出力するステップ間隔。

    static int32_t CELL_NUM;                            //!< シミュレーションで生成するCell数
    static PositionUpdateMethod POSITION_UPDATE_METHOD; //!< 細胞位置の更新方法(陽オイラー法やAdams-Bashforth法など)

    static int32_t GRID_SIZE_MAGNIFICATION; //!< CellListで使用するグリッドサイズの倍率。最小は1、値は2^nである必要がある。
    static int32_t SEARCH_RADIUS;           //!< この半径内(positionの差)にあるcellを力の計算の対象とする。

    static int32_t FIELD_X_LEN; //!< シミュレーションをおこなうフィールドのx方向の辺の長さ。長さは2のn乗とする。
    static int32_t FIELD_Y_LEN; //!< シミュレーションをおこなうフィールドのy方向の辺の長さ。長さは2のn乗とする。
    static int32_t FIELD_Z_LEN; //!< シミュレーションをおこなうフィールドのz方向の辺の長さ。長さは2のn乗とする。

    static std::vector<int64_t> DEFAULT_MOLECULE_NUMS; //!< 分子の初期数(各分子の種類ごとに設定する)
    static int32_t MOLECULE_TYPE_NUM;                  //!< 分子の種類の数

    static int32_t MOLECULE_FIELD_X_LEN; //!< 分子のフィールドのx方向の辺の長さ。長さは2のn乗とする。
    static int32_t MOLECULE_FIELD_Y_LEN; //!< 分子のフィールドのy方向の辺の長さ。長さは2のn乗とする。
    static int32_t MOLECULE_FIELD_Z_LEN; //!< 分子のフィールドのz方向の辺の長さ。長さは2のn乗とする。

    static double DELTA_TIME;          //!< 時間スケール(1が通常時)
    static double MOLECULE_DELTA_TIME; //!< 分子の時間スケール(だいたいDELTA_TIMEより小さい)
};
