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
 * @param _id
 */
Cell::Cell()
  : Cell(0, Vec3(0, 0, 0))
{
}

/**
 * @brief 座標と速度をdouble型で指定して初期化するコンストラクタ。
 *
 * @param _id
 * @param _index
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
 * @param _id
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
 * @brief デストラクタは定義していない。
 *
 */
Cell::~Cell()
{
    cellPool.push(this->arrayIndex);
}

/**
 * @brief Cellの種類をIDで返す。
 *
 * @return int32_t
 */
int32_t Cell::getCellType() const noexcept
{
    return typeID;
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

Vec3 Cell::getVelocity() const noexcept
{
    return velocity;
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
    divisionCycleGauge += 0.1;

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

    Cell c(this->typeID, this->getPosition().x + 5, this->getPosition().y, 10);
    c.addForce(Vec3(-5, 0, 0));
    this->addForce(Vec3(5, 0, 0));

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