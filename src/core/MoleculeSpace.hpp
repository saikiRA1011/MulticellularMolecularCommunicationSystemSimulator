/**
 * @file MoleculeSpace.hpp
 * @author Takanori Saiki
 * @brief
 * @version 0.1
 * @date 2023-07-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "../SimulationSettings.hpp"
#include "../thirdparty/nameof.hpp"
#include "../utils/MakeVector.hpp"
#include "../utils/Util.hpp"
#include <random>

enum class MoleculeDistributionType
{
    UNIFORM,
    GAUSSIAN,
    POINT
};

enum class MoleculeSpaceBorderType
{
    NEUMANN,   // 境界部分を分子が通過できない(その場にとどまる)
    DIRICHLET, // 境界部分で分子が消失する
    PBC,       // 境界部分で分子が反対側に出現する
};

// class Distribution
// {
//   private:
//     MoleculeDistributionType distributionType;

//     std::normal_distribution<double> normalDistribution;
//     std::uniform_real_distribution<double> uniformDistribution;

//   public:
//     Distribution(/* args */);
//     ~Distribution();

//     void setDistributionType(MoleculeDistributionType distributionType);
//     double generate(auto &randGen);
// };

class MoleculeSpace
{
  private:
    int32_t width;       // x方向の格子数(横幅)
    int32_t height;      // y方向の格子数(高さ)
    int32_t depth;       // z方向の格子数(縦幅)
    const double dr;     // 空間の各格子の大きさ
    int64_t moleculeNum; // 現在の分子の総数

    Field3D<double> moleculeSpace; // 分子を扱う空間。各格子に分子の数を格納する。

    const double D; // 拡散係数

    double diffuse(int32_t x, int32_t y, int32_t z);
    double production(int32_t x, int32_t y, int32_t z);
    double decay(int32_t x, int32_t y, int32_t z);
    double advection(int32_t x, int32_t y, int32_t z);

    void setupBoundary(Field3D<double>& ms, MoleculeSpaceBorderType borderType);

  public:
    MoleculeSpace(int64_t moleculeNum, MoleculeDistributionType distributionType);
    ~MoleculeSpace();

    void nextStep() noexcept;

    void print() const noexcept;
};