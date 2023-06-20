/**
 * @file UserCell.hpp
 * @author Takanori Saiki
 * @brief シミュレーション固有のCellの設計をここに示す
 * @version 0.1
 * @date 2023-04-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "core/Cell.hpp"

class UserCell : public Cell
{
  private:
    double divisionTime;      //!< 細胞の分裂周期
    double dieTime;           //!< 細胞の死滅周期
    double divisionGauge = 0; //!< 細胞の分裂周期のゲージ。divisionTimeを超えたら分裂する。
    double dieGauge      = 0; //!< 細胞の死滅周期のゲージ。dieTimeを超えたら死滅する。

    static std::mt19937 randomEngine;
    static std::exponential_distribution<> divisionDist;
    static std::exponential_distribution<> dieDist;

  public:
    UserCell();
    UserCell(CellType _typeID, double x, double y, double radius = 5.0, double vx = 0, double vy = 0);
    UserCell(CellType _typeID, Vec3 pos, double radius = 5.0, Vec3 v = Vec3::zero());
    bool checkWillDivide() const noexcept override;
    bool checkWillDie() const noexcept override;
    void metabolize() noexcept override;
    int32_t die() noexcept override;

    // TODO: Simulation, UserSimulationにおいて、UserCellをどのように使うのかよく考える
    // TODO: これの挙動をよく考える！
    UserCell divide() noexcept;
};

// TODO: 以下のメモをチェックして細胞間相互作用をまとめるクラスを作成する
/*
            CellType
            ↓      ↓
Cell → UserCell → CellCellForce
           ↓           ↓
       Simulator → UserSimulator

CellCellForceは細胞間相互作用を定義するクラス。
UserSimulatorでそれぞれの種類の細胞がどのCellCellForcesを用いるかを条件分岐で定義する。

UserCellで扱う分裂や死滅のモデルはどこで定義する？
細胞ごとに挙動を変えたい。
*/