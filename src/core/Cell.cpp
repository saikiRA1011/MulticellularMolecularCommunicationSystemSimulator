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

int32_t Cell::upperOfCellCount  = 0;
int32_t Cell::numberOfCellsBorn = 0;
std::queue<int> Cell::cellPool  = std::queue<int>();

/**
 * @brief たぶん使わないけど一応作っておく
 *
 */
Cell::Cell()
  : Cell(0, Vec3(0, 0, 0))
{
}

/**
 * @brief 座標と速度をdouble型で指定して初期化するコンストラクタ。
 *
 * @param _typeID
 * @param x
 * @param y
 * @param radius
 * @param vx
 * @param vy
 */
Cell::Cell(int _typeID, double x, double y, double radius, double vx, double vy)
  : Cell(_typeID, Vec3(x, y, 0), radius, Vec3(vx, vy, 0))
{
}

/**
 * @brief
 * 座標と速度をVec3型で指定して初期化するコンストラクタ。呼び出し毎にcellNumをインクリメントする。
 *
 * @param _typeID
 * @param pos
 * @param radius
 * @param v
 */
Cell::Cell(int _typeID, Vec3 pos, double radius, Vec3 v)
  : typeID(_typeID)
  , position(pos)
  , velocity(v)
  , weight(1.0)
  , willDivide(false)
  , id(numberOfCellsBorn)
  , arrayIndex(getNewCellIndex())
  , radius(radius)
  , divisionCycleTime(10)
  , divisionCycleGauge(0)
{
    if (typeID != -1) { // 死んだ細胞じゃなかったら
        numberOfCellsBorn++;
    }
}

/**
 * @brief プールにインデックスを返す。
 *
 */
Cell::~Cell()
{
    cellPool.push(this->arrayIndex);
}

/**
 * @brief 接着しているCellのリストを初期化する。初期化は遅いので注意が必要。必要ならSimulation::stepPreprocessで呼び出す。
 *
 */
void Cell::clearAdhereCells() noexcept
{
    adhereCells.clear();
}

/**
 * @brief Cell cと接着する。内部的には配列に追加するだけ。接着のタイミングは今の所ユーザ定義
 *
 * @param c
 */
void Cell::adhere(const Cell& c) noexcept
{
    adhereCells.emplace_back(&c);
}

void Cell::metabolize() noexcept
{
    divisionCycleGauge += 0.1 * DELTA_TIME; // DELTA_TIMEをかけて時間スケールを合わせる

    if (divisionCycleGauge >= divisionCycleTime) {
        willDivide = true;
    }
}

/**
 * @brief 細胞死。死亡した細胞にtypeを変え、Cellのインデックスを返却する。
 *
 * @return int32_t
 */
int32_t Cell::die() noexcept
{
    typeID = -1;
    cellPool.push(arrayIndex);

    return arrayIndex;
}

// TODO: 分裂後の移動方向は適当に決めているので後でランダムにする。
/**
 * @brief 細胞分裂を起こす。
 *
 * @return Cell
 */
Cell Cell::divide() noexcept
{
    if (!willDivide)
        return Cell(-1, -1, -1);

    willDivide         = false;
    divisionCycleGauge = 0;

    Vec3 pos = this->getPosition();

    Vec3 childDirection = Vec3::randomDirection2();

    Cell c(this->typeID, this->getPosition() + childDirection, 10);
    c.addForce(childDirection);
    this->addForce(-childDirection);

    return c;
}

// TODO: 実装する。
/**
 * @brief moleculeId の分子を周囲環境に放出する。
 *
 * @param moleculeId
 */
void Cell::emitMolecule(int moleculeId) noexcept
{
    std::cout << moleculeId << std::endl;
    return;
}

/**
 * @brief 余っているCellのインデックスを返す。プールが空になっていれば新しいインデックスを生成する。
 *
 * @return int32_t
 * @note 例えば、Cellが分裂したときなどに用いる。cellPoolはCellが死滅した際などに補充される。
 */
int32_t Cell::getNewCellIndex() noexcept
{
    if (Cell::cellPool.empty()) {
        upperOfCellCount++;
        int32_t newIndex = upperOfCellCount;
        return newIndex;
    }

    int32_t newIndex = cellPool.front();
    Cell::cellPool.pop();
    return newIndex;
}

/**
 * @brief Cellの情報をすべて出力する。
 *
 */
void Cell::printCell() const noexcept
{
    std::cout << id << "\t" << typeID << "\t";
    std::cout << position.x << "\t" << position.y << "\t" << position.z << "\t" << velocity.x << "\t" << velocity.y << "\t" << velocity.z << "\t" << radius << "\t" << adhereCells.size();

    for (int i = 0; i < adhereCells.size(); i++) {
        std::cout << adhereCells[i]->id;

        if (i != adhereCells.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
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

/**
 * @brief Cellに割り当てられたIndexをcellPoolに戻す。細胞が完全に消滅した場合に呼び出す。
 *
 * @return int32_t
 */
int32_t Cell::releaseIndex() noexcept
{
    int32_t idx = this->arrayIndex;
    cellPool.push(idx);

    return idx;
}