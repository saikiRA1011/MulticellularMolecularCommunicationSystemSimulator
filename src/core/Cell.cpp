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
  : Cell(CellType::WORKER, Vec3(0, 0, 0))
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
Cell::Cell(CellType _typeID, double x, double y, double radius, double vx, double vy)
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
Cell::Cell(CellType _typeID, Vec3 pos, double radius, Vec3 v)
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
  , dieCycleTime(15)
  , dieCycleGauge(0)
{
    if (!(typeID == CellType::TMP || typeID == CellType::NONE)) { // TMP細胞、NONE細胞はカウントしない
        numberOfCellsBorn++;
    }
}

/**
 * @brief デストラクタは今の所特に何もしない
 *
 */
Cell::~Cell()
{
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

bool Cell::checkWillDie() const noexcept
{
    return (dieCycleGauge >= dieCycleTime);
}

/**
 * @brief Cellが分裂できるかどうかを返す。
 *
 * @return true
 * @return false
 */
bool Cell::checkWillDivide() const noexcept
{
    // アホみたいなモデル。単純に時間経過で分裂する。
    return (divisionCycleGauge >= divisionCycleTime);
}

// TODO: あとでUserCellに移し替える
/**
 * @brief Cellを代謝する。今の所は分裂サイクルゲージを増やすだけ。
 *
 */
void Cell::metabolize() noexcept
{
    divisionCycleGauge += 0.1 * DELTA_TIME; // DELTA_TIMEをかけて時間スケールを合わせる
    dieCycleGauge += 0.1 * DELTA_TIME;      // DELTA_TIMEをかけて時間スケールを合わせる

    double r = this->getRadius();
    this->setRadius(r + 0.03 * DELTA_TIME);
}

/**
 * @brief 細胞死。死亡時の処理(細胞を残すのか、消滅させるのか、あるいは分解されるのか)はユーザが定義する。
 *
 * @return int32_t
 */
int32_t Cell::die() noexcept
{
    typeID = CellType::NONE;

    return releaseIndex();
}

/**
 * @brief 細胞分裂を起こす。
 *
 * @return Cell
 */
Cell Cell::divide() noexcept
{
    divisionCycleGauge = 0;
    willDivide         = false;

    Vec3 pos            = this->getPosition();
    Vec3 childDirection = Vec3::randomDirection2(); // どの方向に分裂するかを決める。分裂元は逆方向に動く。
    // 体積を二分割したときの半径を求める。
    double halfVolumeRadius = this->radius / std::pow(2, 1.0 / 3.0);

    // Cellのtypeはとりあえず継承する形にする
    Cell c(this->typeID, this->getPosition() + childDirection, halfVolumeRadius);

    this->setRadius(halfVolumeRadius); // 分裂元も体積を半分にする

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
        int32_t newIndex = upperOfCellCount;
        upperOfCellCount++;

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
    std::cout << id << "\t" << NAMEOF_ENUM(typeID) << "\t";
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