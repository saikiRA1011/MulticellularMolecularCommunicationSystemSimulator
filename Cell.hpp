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

#ifndef CELL_H
#define CELL_H

#include "Vec3.hpp"
#include <iostream>
#include <random>

/**
 * @class Cell
 * @brief Cell単体の状態を管理するクラス
 */
class Cell
{
    private:
    inline static int cellNum             = 0; //!< 現在のCell数
    inline const static double DELTA_TIME = 0.05; //!< 時間スケール(1が通常時)

    Vec3 position; //!< Cellの座標(x,y,z)
    Vec3 velocity; //!< Cellの速度(x,y,z)

    public:
    Cell();
    Cell(double x, double y, double vx, double vy);
    Cell(Vec3 pos, Vec3 v);
    ~Cell();

    Vec3 getPosition() const noexcept;

    void addForce(double fx, double fy) noexcept;
    void addForce(Vec3 f) noexcept;
    void nextStep() noexcept;

    void printCell() const noexcept;

    // デバッグ用
    void printDebug() const noexcept;

    const int id;        //!< CellのID
    const double radius; //!< Cellの半径
};

#endif // CELL_H

/**
 * @brief 基本となるコンストラクタ。呼び出し毎にcellNumをインクリメントする。
 *
 */
Cell::Cell()
  : position(0, 0, 0)
  , velocity(0, 0, 0)
  , id(cellNum)
  , radius(5.0)
{
    cellNum++;
}

/**
 * @brief
 * 座標と速度をdouble型で指定して初期化するコンストラクタ。呼び出し毎にcellNumをインクリメントする。
 *
 * @param x
 * @param y
 * @param vx
 * @param vy
 */
Cell::Cell(double x, double y, double vx = 0, double vy = 0)
  : position(x, y)
  , velocity(vx, vy)
  , id(cellNum)
  , radius(5.0)
{
    cellNum++;
}

/**
 * @brief
 * 座標と速度をVec3型で指定して初期化するコンストラクタ。呼び出し毎にcellNumをインクリメントする。
 *
 * @param pos
 * @param v
 */
Cell::Cell(Vec3 pos, Vec3 v = Vec3::zero())
  : position(pos)
  , velocity(v)
  , id(cellNum)
  , radius(5.0)
{
    cellNum++;
}

/**
 * @brief デストラクタは定義していない。
 *
 */
Cell::~Cell()
{}

/**
 * @brief Cellの座標を返す。必ず副作用を付けない点に注意。
 *
 * @return Vec3
 */
Vec3 Cell::getPosition() const noexcept
{
    return position;
}

/**
 * @brief Cellに力を加える(double型)。このモデルでは力はそのまま速度になる。
 *
 * @param fx
 * @param fy
 */
void Cell::addForce(double fx, double fy) noexcept
{
    velocity.x = fx;
    velocity.y = fy;
}

/**
 * @brief Cellに力を加える(Vec3型)。このモデルでは力はそのまま速度になる。
 *
 * @param f
 */
void Cell::addForce(Vec3 f) noexcept
{
    velocity = f;
}

/**
 * @brief
 * Cellの位置を更新し、次の時間に進める。このメソッドはすべてのセルにaddForceした後に呼び出すことを想定している。
 *  もしすべてのCellにaddForceしていなかった場合、Cellを呼び出す順番によって挙動が変わってしまう。
 */
void Cell::nextStep() noexcept
{
    position += velocity.timesScalar(DELTA_TIME);

    const int32_t FIELD_WIDTH = 1024;

    // 座標が画面外に出たら、一周回して画面内に戻す
    if (position.x < -(FIELD_WIDTH / 2)) {
        position.x = -(FIELD_WIDTH / 2);
    }
    if (FIELD_WIDTH / 2 <= position.x) {
        position.x = FIELD_WIDTH / 2 - 1;
    }
    if (position.y < -(FIELD_WIDTH / 2)) {
        position.y = -(FIELD_WIDTH / 2);
    }
    if (FIELD_WIDTH / 2 <= position.y) {
        position.y = FIELD_WIDTH / 2 - 1;
    }
}

/**
 * @brief Cellの情報をすべて出力する。
 *
 */
void Cell::printCell() const noexcept
{
    std::cout << id << "\t";
    std::cout << position.x << "\t" << position.y << "\t" << position.z << "\t"
              << velocity.x << "\t" << velocity.y << "\t" << velocity.z
              << std::endl;
}

/**
 * @brief Cellの位置と速度を出力する。デバッグにしか使わない予定。
 *
 */
void Cell::printDebug() const noexcept
{
    std::cout << "x = " << position.x << std::endl;
    std::cout << "y = " << position.y << std::endl;
    std::cout << "vx = " << velocity.x << std::endl;
    std::cout << "vy = " << velocity.y << std::endl;
    std::cout << std::endl;
}