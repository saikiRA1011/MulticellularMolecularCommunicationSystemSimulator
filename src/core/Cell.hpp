/**
 * @file Cell.hpp
 * @author Takanori Saiki
 * @brief Cell class
 * @version 0.1
 * @date 2022-04-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../utils/Vec3.hpp"
#include <iostream>
#include <random>

/**
 * @class Cell
 * @brief Cell単体の状態を管理するクラス
 */
class Cell
{
    private:
    int typeID;
    Vec3 position; //!< Cellの座標(x,y,z)
    Vec3 velocity; //!< Cellの速度(x,y,z)
    double weight; //!< Cellの質量

    double divisionCycleTime;         //!< 細胞の分裂周期
    double divisionCycleGauge;        //!< 細胞の分裂周期のゲージ。divisionCycleTimeを超えたら分裂する。
    std::vector<int> molecularStocks; //!< 細胞の保持している分子数。配列の添字は分子の種類。

    public:
    Cell(int _id);
    Cell(int _id, int _typeID, double x, double y, double radius = 5.0, double vx = 0, double vy = 0);
    Cell(int _id, int _typeID, Vec3 pos, double radius = 5.0, Vec3 v = Vec3::zero());
    ~Cell();

    Vec3 getPosition() const noexcept;
    double getWeight() const noexcept;

    void addForce(double fx, double fy) noexcept;
    void addForce(Vec3 f) noexcept;
    void nextStep() noexcept;

    void emitMolecule(int moleculeId) noexcept;

    void printCell() const noexcept;

    // デバッグ用
    void printDebug() const noexcept;

    const int id;        //!< CellのID
    const double radius; //!< Cellの半径
};
