/**
 * @file Vec3.hpp
 * @author Takanori Saiki
 * @brief 3D Vector class
 * @version 0.1
 * @date 2022-04-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cmath>
#include <iostream>

/**
 * @class Vec3
 * @brief 3次元のベクトルを扱うクラス。
 */
class Vec3
{
    public:
    Vec3();
    Vec3(double x, double y, double z = 0);
    ~Vec3();

    Vec3 operator-() const noexcept;
    Vec3 operator+(Vec3 obj) const noexcept;
    Vec3 operator-(Vec3 obj) const noexcept;
    Vec3 &operator+=(const Vec3 &obj) noexcept;
    Vec3 &operator-=(const Vec3 &obj) noexcept;

    bool operator==(const Vec3 &obj) const noexcept;
    bool operator!=(const Vec3 &obj) const noexcept;

    Vec3 timesScalar(double num) const noexcept; // スカラー倍
    double dot(Vec3 vec) const noexcept;         // 内積
    Vec3 rotate(double theta,
                double phi) const noexcept; // theta, phi = Rad で回転
    Vec3 cross(Vec3 vec) const noexcept;    // 外積
    Vec3 normalize() const noexcept;        // 正規化
    double length() const noexcept;         // 長さ
    double dist(Vec3 vec) const noexcept;   // 距離

    void print() const noexcept;

    static Vec3 zero() noexcept;

    double x, y, z; //!< x, y, z座標
};
