/**
 * @file Molecule.hpp
 * @author Takanori Saiki
 * @brief Molecule class
 * @version 0.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MOLECULE_H
#define MOLECULE_H

#include "Vec3.hpp"
#include <iostream>
#include <random>

/**
 * @class Molecule
 * @brief 分子単体の状態を管理するクラス
 */
class Molecule
{
    private:
    Vec3 position; //!< 分子の座標(x,y,z)
    Vec3 velocity; //!< 分子の速度(x,y,z)

    public:
    Molecule(int _id);
    Molecule(int _id, double x, double y, double vx, double vy);
    Molecule(int _id, Vec3 pos, Vec3 v);
    ~Molecule();

    Vec3 getPosition() const noexcept;

    void addForce(double fx, double fy) noexcept;
    void addForce(Vec3 f) noexcept;
    void nextStep() noexcept;

    void printMolecule() const noexcept;

    // デバッグ用
    void printDebug() const noexcept;

    const int id;        //!< 細胞のID
    const double radius; //!< Cellの半径
};

#endif // MOLECULE_H

/**
 * @brief 基本となるコンストラクタ。呼び出し毎にcellNumをインクリメントする。
 *
 * @param _id
 */
Cell::Cell(int _id)
  : position(0, 0, 0)
  , velocity(0, 0, 0)
  , id(_id)
  , radius(5.0)
{}

/**
 * @brief
 * 座標と速度をdouble型で指定して初期化するコンストラクタ。
 *
 * @param _id
 * @param x
 * @param y
 * @param vx
 * @param vy
 */
Cell::Cell(int _id, double x, double y, double vx = 0, double vy = 0)
  : position(x, y)
  , velocity(vx, vy)
  , id(_id)
  , radius(5.0)
{}

/**
 * @brief
 * 座標と速度をVec3型で指定して初期化するコンストラクタ。呼び出し毎にcellNumをインクリメントする。
 *
 * @param _id
 * @param pos
 * @param v
 */
Cell::Cell(int _id, Vec3 pos, Vec3 v = Vec3::zero())
  : position(pos)
  , velocity(v)
  , id(_id)
  , radius(5.0)
{}

/**
 * @brief デストラクタは定義していない。
 *
 */
Cell::~Cell()
{}

/**
 * @brief 分子の座標を返す。必ず副作用を付けない点に注意。
 *
 * @return Vec3
 */
Vec3 Cell::getPosition() const noexcept
{
    return position;
}

/**
 * @brief 分子に力を加える(double型)。このモデルでは力はそのまま速度になる。
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
 * @brief 分子に力を加える(Vec3型)。このモデルでは力はそのまま速度になる。
 *
 * @param f
 */
void Cell::addForce(Vec3 f) noexcept
{
    velocity = f;
}

/**
 * @brief
 * 分子の位置を更新し、次の時間に進める。このメソッドはすべての分子にaddForceした後に呼び出すことを想定している。
 *  もしすべての分子にaddForceしていなかった場合、分子を呼び出す順番によって挙動が変わってしまう。
 */
void Cell::nextStep() noexcept
{
    position += velocity;

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
 * @brief 分子の情報をすべて出力する。
 *
 */
void Cell::printMolecule() const noexcept
{
    std::cout << id << "\t";
    std::cout << position.x << "\t" << position.y << "\t" << position.z << "\t" << velocity.x << "\t" << velocity.y << "\t" << velocity.z << std::endl;
}

/**
 * @brief 分子の位置と速度を出力する。デバッグにしか使わない予定。
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