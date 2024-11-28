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
#include <memory>
#include <queue>
#include <random>

/**
 * @class Cell
 * @brief Cell単体の状態を管理するクラス
 */
class Cell
{
  protected:
    CellType typeID;
    Vec3 position; //!< Cellの座標(x,y,z)
    Vec3 velocity; //!< Cellの速度(x,y,z)
    double weight; //!< Cellの質量
    double radius; //!< Cellの半径

    std::vector<const Cell*> adhereCells;

    std::vector<int> molecularStocks; //!< 細胞の保持している分子数。配列の添字は分子の種類。

    int32_t releaseIndex() noexcept;

    void adjustPosInField() noexcept;

  private:
    static int32_t upperOfCellCount;
    std::queue<Vec3> preVelocitiesQueue;

    // Simulation *sim; //!< Cellの呼び出し元になるSimulationインスタンスのポインタ

  public:
    Cell();
    Cell(CellType _typeID, double x, double y, double radius = 5.0, double vx = 0, double vy = 0);
    Cell(CellType _typeID, Vec3 pos, double radius = 5.0, Vec3 v = Vec3::zero());
    ~Cell();

    static double calcRadiusFromVolume(double volume) noexcept;
    static double calcVolumeFromRadius(double radius) noexcept;

    CellType getCellType() const noexcept;
    Vec3 getPosition() const noexcept;
    Vec3 getVelocity() const noexcept;
    double getWeight() const noexcept;
    double getRadius() const noexcept;
    double setRadius(double r);

    void initForce() noexcept;
    void addForce(double fx, double fy) noexcept;
    void addForce(Vec3 f) noexcept;
    void nextStep() noexcept;

    void clearAdhereCells() noexcept;
    void adhere(const Cell& c) noexcept;

    virtual bool checkWillDie() const noexcept;    // ユーザが定義
    virtual bool checkWillDivide() const noexcept; // ユーザが定義
    virtual void metabolize() noexcept;            // ユーザが定義
    virtual int32_t die() noexcept;                // ユーザが定義
    Cell divide() noexcept;                        // オーバーロードして使う。

    void emitMolecule(int moleculeId) noexcept;

    static int32_t getNewCellIndex() noexcept;

    void printCell() const noexcept;
    void printDebug() const noexcept; // デバッグ用

    static int32_t numberOfCellsBorn;
    static std::queue<int> cellPool; //!< CellのIDを管理するためのキュー

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
 * @brief Cellにかかっている力を初期化する。
 *
 */
inline void Cell::initForce() noexcept
{
    velocity = Vec3::zero();
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
    velocity += f.timesScalar(1.0 / weight);
}

/**
 * @brief
 * Cellの位置を更新し、次の時間に進める。このメソッドはすべてのセルにaddForceした後に呼び出すことを想定している。
 *  もしすべてのCellにaddForceしていなかった場合、Cellを呼び出す順番によって挙動が変わってしまう。
 */
inline void Cell::nextStep() noexcept
{
    // this->position += velocity;

    int32_t preVelocitiesNum = preVelocitiesQueue.size();

    assert(0 <= preVelocitiesNum && preVelocitiesNum <= 3);

    Vec3 adjustedVelocity = Vec3::zero();

    // 4次Adams-Bashforth法の係数 t-3, t-2,  t-1,   t
    double velocityWeight[4] = { -9.0, 37.0, -59.0, 55.0 };

    // TODO: adams-bashforth法を用いると細胞の挙動がおかしくなる。原因を調べる。
    // https://www1.gifu-u.ac.jp/~tanaka/numerical_analysis.pdf Adams-bashforth法は安定性があまりよくないらしい

    // 初回は過去の速度がないので、現在の速度で初期化する。
    // 本当ならルンゲクッタ法を使って初期化したほうがいい
    if (preVelocitiesNum == 0) {
        for (int32_t i = 0; i < 3; i++) {
            preVelocitiesQueue.push(velocity);
        }
    }

    // 4次のAdams-Bashforth法
    adjustedVelocity += velocity.timesScalar(velocityWeight[3]);
    Vec3 preVelocity;
    for (int32_t i = 0; i < 3; i++) {
        preVelocity = preVelocitiesQueue.front();
        preVelocitiesQueue.pop();

        if (i != 0) { // キューの先頭(一番古いデータ)以外は再びキューに追加する。一番古いデータはもう使わないので削除
            preVelocitiesQueue.push(preVelocity);
        }

        adjustedVelocity += preVelocity.timesScalar(velocityWeight[i]); // 重みをかけて足す
    }

    adjustedVelocity = adjustedVelocity.timesScalar(1.0 / 24.0);
    position += adjustedVelocity; // 2次のルンゲクッタ法

    preVelocitiesQueue.push(velocity); // 今回の速度をキューに追加(調整前)

    adjustPosInField();
}