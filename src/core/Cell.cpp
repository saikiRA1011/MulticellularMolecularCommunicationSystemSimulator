/**
 * @file Cell.cpp
 * @author Takanori Saiki
 * @brief Cell class cpp file
 * @version 0.1
 * @date 2022-06-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Cell.hpp"

/**
 * @brief 基本となるコンストラクタ。
 *
 * @param _id
 */
Cell::Cell(int _id)
  : typeID(0)
  , position(0, 0, 0)
  , velocity(0, 0, 0)
  , weight(1.0)
  , id(_id)
  , radius(5.0)
{
}

/**
 * @brief 座標と速度をdouble型で指定して初期化するコンストラクタ。
 *
 *
 * @param _id
 * @param _typeID
 * @param x
 * @param y
 * @param radius
 * @param vx
 * @param vy
 */
Cell::Cell(int _id, int _typeID, double x, double y, double radius, double vx, double vy)
  : typeID(_typeID)
  , position(x, y)
  , velocity(vx, vy)
  , weight(1.0)
  , id(_id)
  , radius(radius)
{
}

/**
 * @brief
 * 座標と速度をVec3型で指定して初期化するコンストラクタ。呼び出し毎にcellNumをインクリメントする。
 *
 * @param _id
 * @param _typeID
 * @param pos
 * @param radius
 * @param v
 */
Cell::Cell(int _id, int _typeID, Vec3 pos, double radius, Vec3 v)
  : typeID(_typeID)
  , position(pos)
  , velocity(v)
  , weight(1.0)
  , id(_id)
  , radius(radius)
{
}

/**
 * @brief デストラクタは定義していない。
 *
 */
Cell::~Cell()
{
}

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
 * @brief Cellの質量を返す。
 *
 * @return double
 */
double Cell::getWeight() const noexcept
{
    return weight;
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

void Cell::emitMolecule(int moleculeId) noexcept
{
    std::cout << moleculeId << std::endl;
    return;
}

/**
 * @brief Cellの情報をすべて出力する。
 *
 */
void Cell::printCell() const noexcept
{
    std::cout << id << "\t" << typeID << "\t";
    std::cout << position.x << "\t" << position.y << "\t" << position.z << "\t" << velocity.x << "\t" << velocity.y << "\t" << velocity.z << "\t" << radius << std::endl;
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