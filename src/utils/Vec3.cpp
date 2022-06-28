#include "Vec3.hpp"

/**
 * @brief 基本となるコンストラクタ。すべての成分を0に設定する。
 *
 */
Vec3::Vec3()
{
    x = 0;
    y = 0;
    z = 0;
}

/**
 * @brief x, y, z成分を指定して初期化するコンストラクタ。zはデフォルトで0。
 *
 * @param x
 * @param y
 * @param z (default: 0)
 */
Vec3::Vec3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

/**
 * @brief いまのところ何もしない。
 *
 */
Vec3::~Vec3()
{
}

/**
 * @brief 逆符号のベクトルを返す。
 *
 * @return Vec3
 */
Vec3 Vec3::operator-() const noexcept
{
    Vec3 tmp(-x, -y, -z);

    return tmp;
}

/**
 * @brief ベクトル同士を加算する。
 *
 * @param obj
 * @return Vec3
 */
Vec3 Vec3::operator+(Vec3 obj) const noexcept
{
    Vec3 tmp(x, y, z);
    tmp += obj;

    return tmp;
}

/**
 * @brief ベクトル同士を減算する。
 *
 * @param obj
 * @return Vec3
 */
Vec3 Vec3::operator-(Vec3 obj) const noexcept
{
    Vec3 tmp(x, y, z);
    tmp -= obj;

    return tmp;
}

/**
 * @brief ベクトルを定数倍する。
 *
 * @param num
 * @return Vec3
 */
Vec3 Vec3::timesScalar(const double num) const noexcept
{
    const Vec3 tmp(x * num, y * num, z * num);

    return tmp;
}

Vec3 &Vec3::operator+=(const Vec3 &obj) noexcept
{
    x += obj.x;
    y += obj.y;
    z += obj.z;

    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &obj) noexcept
{
    x -= obj.x;
    y -= obj.y;
    z -= obj.z;

    return *this;
}

/**
 * @brief ベクトルの内積を返す。
 *
 * @param vec
 * @return double
 */
double Vec3::dot(Vec3 vec) const noexcept
{
    return x * vec.x + y * vec.y + z * vec.z;
}

/**
 * @brief ベクトルを方位角theta,
 * 天頂角phiで回転させたベクトルを返す。若干重い定数倍計算量なので注意。
 *
 * @param theta
 * @param phi
 * @return Vec3
 */
Vec3 Vec3::rotate(double theta, double phi) const noexcept
{
    double rotatedPos[3] = { x, y, z };

    double rotateZ[3][3] = { { std::cos(theta), -std::sin(theta), 0 }, { std::sin(theta), std::cos(theta), 0 }, { 0, 0, 1 } };
    double rotateY[3][3] = { { std::cos(phi), 0, std::sin(phi) }, { 0, 1, 0 }, { -std::sin(phi), 0, std::cos(phi) } };

    for (int y_i = 0; y_i < 3; y_i++) {
        double sum    = 0;
        double tmp[3] = { rotatedPos[0], rotatedPos[1], rotatedPos[2] };

        for (int x_i = 0; x_i < 3; x_i++) {
            sum = rotateZ[y_i][x_i] * tmp[x_i];
        }
        rotatedPos[y_i] = sum;
    }

    for (int y_i = 0; y_i < 3; y_i++) {
        double sum    = 0;
        double tmp[3] = { rotatedPos[0], rotatedPos[1], rotatedPos[2] };

        for (int x_i = 0; x_i < 3; x_i++) {
            sum = rotateY[y_i][x_i] * tmp[x_i];
        }
        rotatedPos[y_i] = sum;
    }

    Vec3 tmp(rotatedPos[0], rotatedPos[1], rotatedPos[2]);

    return tmp;
}

/**
 * @brief ベクトルの外積を返す。
 *
 * @param vec
 * @return Vec3
 */
Vec3 Vec3::cross(Vec3 vec) const noexcept
{
    Vec3 tmp(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);

    return tmp;
}

/**
 * @brief ベクトルを正規化する。ただし、長さが0の場合は0を返す。
 *
 * @return Vec3
 */
Vec3 Vec3::normalize() const noexcept
{
    double length = this->length();

    if (length == 0.0) {
        return Vec3(0, 0, 0);
    }

    Vec3 tmp(x / length, y / length, z / length);

    return tmp;
}

/**
 * @brief ベクトルの長さを返す。
 *
 * @return double
 */
double Vec3::length() const noexcept
{
    return std::sqrt(x * x + y * y + z * z);
}

/**
 * @brief ベクトル同士の距離を返す。
 *
 * @param vec
 * @return double
 */
double Vec3::dist(Vec3 vec) const noexcept
{
    Vec3 diff = *this - vec;

    return diff.length();
}

/**
 * @brief ベクトルの成分を出力する。デバッグ用途。
 *
 */
void Vec3::print() const noexcept
{
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}

/**
 * @brief ゼロベクトルを返す。
 *
 * @return Vec3
 */
Vec3 Vec3::zero() noexcept
{
    return Vec3(0, 0, 0);
}