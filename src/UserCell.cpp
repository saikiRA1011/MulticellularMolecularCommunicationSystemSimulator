#include "UserCell.hpp"

std::mt19937 UserCell::randomEngine(1231);
std::exponential_distribution<> UserCell::divisionDist(1.0 / 10.0);
std::exponential_distribution<> UserCell::dieDist(1.0 / 15.0);

// TODO: 現在のモデルを実行すると左上の方にあるさいぼうが

UserCell::UserCell()
  : UserCell(CellType::WORKER, Vec3(0, 0, 0))
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
UserCell::UserCell(CellType _typeID, double x, double y, double radius, double vx, double vy)
  : UserCell(_typeID, Vec3(x, y, 0), radius, Vec3(vx, vy, 0))
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
UserCell::UserCell(CellType _typeID, Vec3 pos, double radius, Vec3 v)
  : Cell(_typeID, pos, radius, v)
  , divisionGauge{ 0 }
  , dieGauge{ 0 }
{
    divisionTime = divisionDist(randomEngine);
    dieTime      = dieDist(randomEngine);
}

bool UserCell::checkWillDivide() const noexcept
{
    if (divisionGauge >= divisionTime) {
        return true;
    }
    return false;
}

bool UserCell::checkWillDie() const noexcept
{
    if (dieGauge >= dieTime) {
        return true;
    }
    return false;
}

void UserCell::metabolize() noexcept
{
    divisionGauge += 0.1 * DELTA_TIME; // DELTA_TIMEをかけて時間スケールを合わせる
    dieGauge += 0.1 * DELTA_TIME;      // DELTA_TIMEをかけて時間スケールを合わせる

    double r = this->getRadius();
    this->setRadius(r + 0.03 * DELTA_TIME);
}

int32_t UserCell::die() noexcept
{
    dieGauge = 0;

    return Cell::die();
}

UserCell UserCell::divide() noexcept
{
    divisionGauge = 0;
    divisionTime  = divisionDist(randomEngine);

    Vec3 pos            = this->getPosition();
    Vec3 childDirection = Vec3::randomDirection2().timesScalar(5); // どの方向に分裂するかを決める。分裂元は逆方向に動く。

    // 体積を二分割したときの半径を求める。
    double halfVolumeRadius = this->radius / std::pow(2, 1.0 / 3.0);

    // Cellのtypeはとりあえず継承する形にする
    UserCell c(this->typeID, this->getPosition() + childDirection, halfVolumeRadius);
    c.adjustPosInField();

    this->setRadius(halfVolumeRadius); // 分裂元も体積を半分にする

    c.addForce(childDirection);
    this->addForce(-childDirection);

    return c;
}