#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class Vec2
{
    public:
    Vec2();
    Vec2(double x, double y);
    ~Vec2();

    Vec2 operator-();
    Vec2 operator+(Vec2 obj);
    Vec2 operator-(Vec2 obj);
    Vec2 &operator+=(Vec2 &obj);
    Vec2 &operator-=(Vec2 &obj);

    Vec2 timesScalar(double num); // スカラー倍
    double dot(Vec2 vec);         // 内積
    Vec2 rotate(double theta);    // theta = Rad で回転

    static Vec2 zero();

    double x, y;
};

#endif VEC2_H

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2::Vec2(double x, double y)
{
    this->x = x;
    this->y = y;
}

Vec2::~Vec2()
{
    delete (this);
}

Vec2 Vec2::operator-()
{
    Vec2 tmp(-x, -y);

    return tmp;
}

Vec2 Vec2::operator+(Vec2 obj)
{
    Vec2 tmp(x, y);
    tmp += obj;

    return tmp;
}

Vec2 Vec2::operator-(Vec2 obj)
{
    Vec2 tmp(x, y);
    tmp -= obj;

    return obj;
}

Vec2 Vec2::timesScalar(double num)
{
    Vec2 tmp(x * num, y * num);

    return tmp;
}

Vec2 &Vec2::operator+=(Vec2 &obj)
{
    x += obj.x;
    y += obj.y;

    return *this;
}

Vec2 &Vec2::operator-=(Vec2 &obj)
{
    x -= obj.x;
    y -= obj.y;

    return *this;
}

double Vec2::dot(Vec2 vec)
{
    return x * vec.x + y * vec.y;
}

Vec2 Vec2::rotate(double theta)
{
    double tmpX = x * std::cos(theta) - y * std::sin(theta);
    double tmpY = y * std::sin(theta) + y * std::cos(theta);
    Vec2 tmp(tmpX, tmpY);

    return tmp;
}

Vec2 Vec2::zero()
{
    return Vec2(0, 0);
}
