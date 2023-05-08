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

#include "../CellType.hpp"
#include "../SimulationSettings.hpp"
#include "../thirdparty/nameof.hpp"
#include "../utils/Vec3.hpp"
#include <iostream>
#include <queue>
#include <random>

/**
 * @class Cell
 * @brief Cell単体の状態を管理するクラス
 */
class Cell
{
  private:
    CellType typeID;
    Vec3 position; //!< Cellの座標(x,y,z)
    Vec3 velocity; //!< Cellの速度(x,y,z)
    double weight; //!< Cellの質量
    double radius; //!< Cellの半径

    bool willDivide;

    static int32_t upperOfCellCount;
    static int32_t numberOfCellsBorn;

    std::vector<const Cell*> adhereCells;

    double divisionCycleTime;         //!< 細胞の分裂周期
    double divisionCycleGauge;        //!< 細胞の分裂周期のゲージ。divisionCycleTimeを超えたら分裂する。
    std::vector<int> molecularStocks; //!< 細胞の保持している分子数。配列の添字は分子の種類。

    int32_t releaseIndex() noexcept;

    // Simulation *sim; //!< Cellの呼び出し元になるSimulationインスタンスのポインタ

  public:
    static std::queue<int> cellPool; //!< CellのIDを管理するためのキュー

    Cell();
    Cell(CellType _typeID, double x, double y, double radius = 5.0, double vx = 0, double vy = 0);
    Cell(CellType _typeID, Vec3 pos, double radius = 5.0, Vec3 v = Vec3::zero());
    ~Cell();

    CellType getCellType() const noexcept;
    Vec3 getPosition() const noexcept;
    Vec3 getVelocity() const noexcept;
    double getWeight() const noexcept;
    double getRadius() const noexcept;
    double setRadius(double r);

    void addForce(double fx, double fy) noexcept;
    void addForce(Vec3 f) noexcept;
    void nextStep() noexcept;

    void clearAdhereCells() noexcept;
    void adhere(const Cell& c) noexcept;

    virtual bool checkWillDie() const noexcept;    // ユーザが定義
    virtual bool checkWillDivide() const noexcept; // ユーザが定義
    virtual void metabolize() noexcept;            // ユーザが定義
    virtual int32_t die() noexcept;                // ユーザが定義
    Cell divide() noexcept;

    void emitMolecule(int moleculeId) noexcept;

    static int32_t getNewCellIndex() noexcept;

    void printCell() const noexcept;
    void printDebug() const noexcept; // デバッグ用

    const int id;         //!< CellのID
    const int arrayIndex; //!< 配列のどこに入るか
};

/**
 * @brief CellのIDを返す。必ず副作用をつけない点に注意。
 *
 * @return int CellのID
 */
inline CellType Cell::getCellType() const noexcept
{
    return typeID;
}

/**
 * @brief Cellの座標を返す。必ず副作用をつけない点に注意。
 *
 * @return Vec3 Cellの座標
 */
inline Vec3 Cell::getPosition() const noexcept
{
    return position;
}

/**
 * @brief Cellの速度を返す。必ず副作用をつけない点に注意。
 *
 * @return Vec3 Cellの速度
 */
inline Vec3 Cell::getVelocity() const noexcept
{
    return velocity;
}

/**
 * @brief Cellの質量を返す。必ず副作用をつけない点に注意。
 *
 * @return double Cellの質量
 */
inline double Cell::getWeight() const noexcept
{
    return weight;
}

/**
 * @brief Cellの半径を返す。必ず副作用をつけない点に注意。
 *
 * @return double Cellの半径
 */
inline double Cell::getRadius() const noexcept
{
    return radius;
}

/**
 * @brief Cellの半径を設定する。
 *
 * @param r Cellの半径
 * @return double Cellの半径
 */
inline double Cell::setRadius(double r)
{
    if (r < 0.0) {
        throw std::invalid_argument("Cell::setRadius() : radius must be positive.");
    }
    radius = r;

    return radius;
}

/**
 * @brief Cellに力を加える(double型)。このモデルでは力はそのまま速度になる。
 *
 * @param fx x方向の力
 * @param fy y方向の力
 */
inline void Cell::addForce(double fx, double fy) noexcept
{
    velocity.x += fx / weight;
    velocity.y += fy / weight;
}

/**
 * @brief Cellに力を加える(Vec3型)。このモデルでは力はそのまま速度になる。
 *
 * @param f
 */
inline void Cell::addForce(Vec3 f) noexcept
{
    velocity = f.timesScalar(1.0 / weight);
}

/**
 * @brief
 * Cellの位置を更新し、次の時間に進める。このメソッドはすべてのセルにaddForceした後に呼び出すことを想定している。
 *  もしすべてのCellにaddForceしていなかった場合、Cellを呼び出す順番によって挙動が変わってしまう。
 */
inline void Cell::nextStep() noexcept
{
    position += velocity;

    const int32_t FIELD_WIDTH = FIELD_X_LEN;

    // 座標が画面外に出たら、一周回して画面内に戻す
    if (position.x < -(FIELD_WIDTH / 2)) {
        position.x = (FIELD_WIDTH / 2) - 1;
    }
    if (FIELD_WIDTH / 2 <= position.x) {
        position.x = -FIELD_WIDTH / 2;
    }
    if (position.y < -(FIELD_WIDTH / 2)) {
        position.y = (FIELD_WIDTH / 2) - 1;
    }
    if (FIELD_WIDTH / 2 <= position.y) {
        position.y = -FIELD_WIDTH / 2;
    }
}