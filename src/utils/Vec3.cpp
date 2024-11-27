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
    return Vec3(x + obj.x, y + obj.y, z + obj.z);
}

/**
 * @brief ベクトル同士を減算する。
 *
 * @param obj
 * @return Vec3
 */
Vec3 Vec3::operator-(Vec3 obj) const noexcept
{
    return Vec3(x - obj.x, y - obj.y, z - obj.z);
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
 * @brief 単純に、ベクトルのすべての成分が一致しているかどうかを返す。==は完全一致のときのみtrue。
 *
 * @param obj
 * @return true
 * @return false
 *
 * @note 実際には、ベクトルの要素はdouble型で実装されているため数学的には一致する値も内部的には一致しないことがある。
 */
bool Vec3::operator==(const Vec3 &obj) const noexcept
{
    return (x == obj.x && y == obj.y && z == obj.z);
}

/**
 * @brief 単純に、ベクトルのすべての成分が一致しているかどうかを返す。!=はいずれかでも不一致なときにtrue。
 *
 * @param obj
 * @return true
 * @return false
 * @note 実際には、ベクトルの要素はdouble型で実装されているため数学的には一致する値も内部的には一致しないことがある。
 */
bool Vec3::operator!=(const Vec3 &obj) const noexcept
{
    return (x != obj.x || y != obj.y || z != obj.z);
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
 * @note これ未完成なところがあるので注意。使う必要もない気がする。
 */
Vec3 Vec3::rotate(double theta, double phi) const noexcept
{
    double rotatedPos[3] = { x, y, z };

    double rotate[3][3] = { { std::cos(theta) * std::cos(phi), std::sin(theta) * std::cos(phi), -std::sin(phi) },
                            { -std::sin(theta), std::cos(theta), 0 },
                            { std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi) } };

    double v[3] = { rotatedPos[0], rotatedPos[1], rotatedPos[2] };
    for (int y_i = 0; y_i < 3; y_i++) {
        double sum = 0;

        for (int x_i = 0; x_i < 3; x_i++) {
            sum += rotate[y_i][x_i] * v[x_i];
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

/**
 * @brief ランダムの方向(二次元単位ベクトル)を返す。
 *
 */
Vec3 Vec3::randomDirection2() noexcept
{
    static std::mt19937 rand_gen(0);                          //!< 乱数生成器(生成器はとりあえずメルセンヌ・ツイスタ)
    std::uniform_real_distribution<double> angle_ratio(0, 1); //!< 方向の割合
    double angle = 2.0 * M_PI * angle_ratio(rand_gen);

    return Vec3(std::cos(angle), std::sin(angle), 0);
}

// TODO: 動作未確認！テスト作るべし!
/**
 * @brief ランダムの方向(三次元単位ベクトル)を返す。
 *
 * @return Vec3
 */
Vec3 Vec3::randomDirection3() noexcept
{
    static std::mt19937 rand_gen(0);                          //!< 乱数生成器(生成器はとりあえずメルセンヌ・ツイスタ)
    std::uniform_real_distribution<double> angle_ratio(0, 1); //!< 方向の割合
    double theta = 2.0 * M_PI * angle_ratio(rand_gen);
    double phi   = 2.0 * M_PI * angle_ratio(rand_gen);

    return Vec3(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi));
}