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

    double cAMP               = 0;   //!< 細胞内のcAMP濃度
    double activeReceptor     = 1.0; //!< 細胞内の活性受容体の数。割合なので1が最大
    double diffCamp           = 0;
    double diffActiveReceptor = 0;

    static std::mt19937 randomEngine;
    static std::exponential_distribution<> divisionDist;
    static std::exponential_distribution<> dieDist;

    static constexpr double C       = 10;
    static constexpr double K1      = 0.036;
    static constexpr double K2      = 0.666;
    static constexpr double L1      = 10;
    static constexpr double L2      = 0.005;
    static constexpr double Q       = 4000;
    static constexpr double SIGMA   = 6.0;
    static constexpr double Ki      = 1.7;
    static constexpr double Ke      = 5.4;
    static constexpr double Kt      = 0.9;
    static constexpr double ALPHA   = 1;
    static constexpr double H       = 5;
    static constexpr double THETA   = 0.01;
    static constexpr double LAMBDA  = 0.01;
    static constexpr double EPSILON = 1;

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

    double emitMolecule(int32_t moleculeId) noexcept override;
    double absorbMolecule(int32_t moleculeId, double amountOnTheSpot) noexcept override;
    double calcSynthesis(double extracellularCAMP) const noexcept;
    double f1(double cAMP) const noexcept;
    double f2(double cAMP) const noexcept;
    double calcInactiveReceptor(double extracellularCAMP) const noexcept;
    double calcActiveReceptor(double extracellularCAMP) const noexcept;

    void setDiffState(double extracellularCAMP) noexcept;
    void updateState(double extracellularCAMP) noexcept;

    void test(double cAMP) const noexcept;
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