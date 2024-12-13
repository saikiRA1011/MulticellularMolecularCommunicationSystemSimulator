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

// static変数の初期化
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
  , radius(radius)
  , id(numberOfCellsBorn)
  , arrayIndex(getNewCellIndex())
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
 * @brief Cellの速度を計算する。引数には今までの速度を格納したキューを渡す。
 * @note キューの長さは計算アルゴリズムによって変わる。ユーザ側はあまり考えなくていいが、コントリビューターは注意すること。
 *
 * @param velocities
 * @return Vec3
 */
Vec3 Cell::calcVelocity(std::queue<Vec3>& velocities) noexcept
{
    // パラメータに合わせて計算方法を変える
    switch (SimulationSettings::POSITION_UPDATE_METHOD) {
        case PositionUpdateMethod::EULER:
            return calcEuler(velocities);
        case PositionUpdateMethod::AB2:
            return calcAB2(velocities);
        case PositionUpdateMethod::AB3:
            return calcAB3(velocities);
        case PositionUpdateMethod::AB4:
            return calcAB4(velocities);
        case PositionUpdateMethod::ORIGINAL:
            return calcOriginal(velocities);
        default:
            std::cerr << "Error: Invalid PositionUpdateMethod" << std::endl;
            exit(1);
    }
}

/**
 * @brief 速度を4次のAdams-Bashforth法で計算する。
 * @note キューの長さは4である必要がある。また、Adams-Bashforth法は次数が高いほど精度も高くなる反面安定性が下がるので、荒い時間ステップで計算するときは別のアルゴリズムを用いる。
 *       参考：https://www1.gifu-u.ac.jp/~tanaka/numerical_analysis.pdf
 * @param velocities
 * @return Vec3
 */
Vec3 Cell::calcAB4(std::queue<Vec3>& velocities) noexcept
{
    assert(velocities.size() == 4);

    // 4次Adams-Bashforth法の係数 t-3, t-2,  t-1, t
    double velocityWeight[4] = { -9.0, 37.0, -59.0, 55.0 };
    Vec3 adjustedVelocity    = Vec3::zero();

    u_int32_t velocitiesSize = velocities.size();
    Vec3 velocity;
    for (u_int32_t i = 0; i < velocitiesSize; i++) {
        velocity = velocities.front();
        velocities.pop();
        velocities.push(velocity);

        adjustedVelocity += velocity.timesScalar(velocityWeight[i]);
    }
    adjustedVelocity = adjustedVelocity.timesScalar(1.0 / 24.0);

    return adjustedVelocity;
}

/**
 * @brief 速度を3次のAdams-Bashforth法で計算する。
 * @note 詳しいことはcalcAB4()を参照。
 *
 * @param velocities
 * @return Vec3
 */
Vec3 Cell::calcAB3(std::queue<Vec3>& velocities) noexcept
{
    assert(velocities.size() == 3);

    // 3次Adams-Bashforth法の係数 t-2, t-1, t
    double velocityWeight[3] = { 5, -16, 23 };
    Vec3 adjustedVelocity    = Vec3(0, 0, 0);

    u_int32_t velocitiesSize = velocities.size();
    Vec3 velocity;
    for (u_int32_t i = 0; i < velocitiesSize; i++) {
        velocity = velocities.front();
        velocities.pop();
        velocities.push(velocity);

        adjustedVelocity += velocity.timesScalar(velocityWeight[i]);
    }
    adjustedVelocity = adjustedVelocity.timesScalar(1.0 / 12.0);

    return adjustedVelocity;
}

/**
 * @brief 速度を2次のAdams-Bashforth法で計算する。
 * @note 詳しいことはcalcAB4()を参照。
 *
 * @param velocities
 * @return Vec3
 */
Vec3 Cell::calcAB2(std::queue<Vec3>& velocities) noexcept
{
    assert(velocities.size() == 2);

    // 2次Adams-Bashforth法の係数 t-1, t
    double velocityWeight[2] = { -1, 3 };
    Vec3 adjustedVelocity    = Vec3(0, 0, 0);

    u_int32_t velocitiesSize = velocities.size();
    Vec3 velocity;
    for (u_int32_t i = 0; i < velocitiesSize; i++) {
        velocity = velocities.front();
        velocities.pop();
        velocities.push(velocity);

        adjustedVelocity += velocity.timesScalar(velocityWeight[i]);
    }
    adjustedVelocity = adjustedVelocity.timesScalar(1.0 / 2.0);

    return adjustedVelocity;
}

/**
 * @brief 速度をオイラー法で計算する。
 * @note 精度は一番低いが、安定性はあるので荒い時間ステップで計算する場合はこれがいいかもしれない。
 *
 * @param velocities
 * @return Vec3
 */
Vec3 Cell::calcEuler(std::queue<Vec3>& velocities) noexcept
{
    assert(velocities.size() == 1);
    return velocities.front();
}

/**
 * @brief オリジナルの方法で速度を計算する。実際のところはキューの長さに応じて上記の関数を呼び出しているだけ。あまり精度が良くないため使わないほうがいいかも
 *
 * @param velocities
 * @return Vec3
 */
Vec3 Cell::calcOriginal(std::queue<Vec3>& velocities) noexcept
{
    assert(1 <= velocities.size() && velocities.size() <= 4);

    switch (velocities.size()) {
        case 1:
            return calcEuler(velocities);
            break;
        case 2:
            return calcAB2(velocities);
            break;
        case 3:
            return calcAB3(velocities);
            break;
        case 4:
            return calcAB4(velocities);
            break;
        default:
            std::cerr << "Error: Cell::calcOriginal: velocities.size() is invalid." << std::endl;
            exit(1);
            break;
    }
}

/**
 * @brief Cellの体積から半径を計算する
 *
 * @param volume
 * @return double
 */
double Cell::calcRadiusFromVolume(double volume) noexcept
{
    return std::pow(volume * 3.0 / (4.0 * M_PI), 1.0 / 3.0);
}

/**
 * @brief Cellの半径から体積を計算する
 *
 * @param radius
 * @return double
 */
double Cell::calcVolumeFromRadius(double radius) noexcept
{
    return 4.0 / 3.0 * M_PI * std::pow(radius, 3.0);
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
    return false;
}

/**
 * @brief Cellが分裂できるかどうかを返す。
 *
 * @return true
 * @return false
 */
bool Cell::checkWillDivide() const noexcept
{
    return false;
}

// TODO: あとでUserCellに移し替える
/**
 * @brief Cellを代謝する。今の所は分裂サイクルゲージを増やすだけ。
 *
 */
void Cell::metabolize() noexcept
{
    double r = this->getRadius();
    this->setRadius(r + 0.03 * SimulationSettings::DELTA_TIME);
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
double Cell::emitMolecule(int moleculeId) noexcept
{
    return 0.0;
}

/**
 * @brief moleculeId の分子を環境から吸収する
 *
 * @param moleculeId
 * @param amountOnTheSpot
 * @return double
 */
double Cell::absorbMolecule(int moleculeId, double amountOnTheSpot) noexcept
{
    return 0.0;
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
    std::cout << position.x << "\t" << position.y << "\t" << position.z << "\t" << velocity.x << "\t" << velocity.y << "\t" << velocity.z << "\t" << radius << "\t" << adhereCells.size() << "\t";

    if (adhereCells.size() == 0) {
        std::cout << "_";
    }

    // std::cout << id << "\t";
    // std::cout << position.x << "\t" << position.y;

    for (int i = 0; i < (int)adhereCells.size(); i++) {
        std::cout << adhereCells[i]->id;

        if (i != (int)adhereCells.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << "\n";
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

/**
 * @brief Cellの位置をシミュレーションフィールド内に戻す
 *
 */
void Cell::adjustPosInField() noexcept
{
    const int32_t FIELD_WIDTH = SimulationSettings::FIELD_X_LEN;

    // 座標が画面外に出たら、一周回して画面内に戻す
    if (position.x < -(double)(FIELD_WIDTH / 2)) {
        position.x = (FIELD_WIDTH / 2) - 1;
    }
    if ((double)(FIELD_WIDTH / 2) <= position.x) {
        position.x = -FIELD_WIDTH / 2;
    }
    if (position.y < -(double)(FIELD_WIDTH / 2)) {
        position.y = (FIELD_WIDTH / 2) - 1;
    }
    if ((double)(FIELD_WIDTH / 2) <= position.y) {
        position.y = -FIELD_WIDTH / 2;
    }
}
