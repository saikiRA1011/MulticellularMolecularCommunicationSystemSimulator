#include "UserCell.hpp"

std::mt19937 UserCell::randomEngine(1231);
std::exponential_distribution<> UserCell::divisionDist(1.0 / 130.0);
std::exponential_distribution<> UserCell::dieDist(1.0 / 150.0);

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

/**
 * @brief 細胞が分裂するかどうかの判定を行う。
 *
 * @return true
 * @return false
 */
bool UserCell::checkWillDivide() const noexcept
{
    if (divisionGauge >= divisionTime) {
        return true;
    }
    return false;
}

/**
 * @brief 細胞が死滅するかどうかの判定を行う。
 *
 * @return true
 * @return false
 */
bool UserCell::checkWillDie() const noexcept
{
    if (dieGauge >= dieTime) {
        return true;
    }
    return false;
}

/**
 * @brief 細胞の代謝を行う。
 *
 */
void UserCell::metabolize() noexcept
{
    divisionGauge += SimulationSettings::DELTA_TIME; // DELTA_TIMEをかけて時間スケールを合わせる
    dieGauge += SimulationSettings::DELTA_TIME;      // DELTA_TIMEをかけて時間スケールを合わせる

    double r               = this->getRadius();
    const double newVolume = calcVolumeFromRadius(r) + 50.0 * SimulationSettings::DELTA_TIME;
    const double newRadius = calcRadiusFromVolume(newVolume);

    this->setRadius(newRadius);
}

/**
 * @brief 細胞死。死亡時の処理(細胞を残すのか、消滅させるのか、あるいは分解されるのか)はユーザが定義する。
 *
 * @return int32_t
 */
int32_t UserCell::die() noexcept
{
    dieGauge = 0;

    return Cell::die();
}

/**
 * @brief 細胞分裂。分裂時の処理(分裂後の細胞の初期化など)はユーザが定義する。
 *
 * @return UserCell
 */
UserCell UserCell::divide() noexcept
{
    divisionGauge = 0;
    divisionTime  = divisionDist(randomEngine);

    // 体積を二分割したときの半径を求める。
    double halfVolumeRadius = this->radius / std::pow(2, 1.0 / 3.0);

    Vec3 pos            = this->getPosition();
    Vec3 childDirection = Vec3::randomDirection2(); // どの方向に分裂するかを決める。分裂元は逆方向に動く。

    // Cellのtypeはとりあえず継承する形にする。位置はchildDirection方向に半径の半分だけずらす
    UserCell c(this->typeID, this->getPosition() + childDirection.timesScalar(halfVolumeRadius / 2), halfVolumeRadius);
    c.adjustPosInField();

    this->setRadius(halfVolumeRadius); // 分裂元も体積を半分にする

    // c.addForce(childDirection);
    // this->addForce(-childDirection);

    return c;
}

double UserCell::emitMolecule(int32_t moleculeId) noexcept
{
    return 0;
}

double UserCell::absorbMolecule(int32_t moleculeId, double amountOnTheSpot) noexcept
{
    return 0;
}