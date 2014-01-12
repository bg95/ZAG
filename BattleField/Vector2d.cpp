#include <cmath>
#include "Vector2d.h"
#include <Qt>

Vector2d::Vector2d()
{
}

Vector2d::Vector2d(double _x, double _y)
{
    c[0] = _x;
    c[1] = _y;
}

Vector2d &Vector2d::operator =(Vector2d b)
{
    c[0] = b.c[0];
    c[1] = b.c[1];
    return *this;
}

double Vector2d::abs()
{
    return sqrt(x * x + y * y);
}

double Vector2d::arg()
{
    return atan2(y, x);
}

Vector2d Vector2d::unit() {
    double length = this->abs();
    return Vector2d(x / length, y / length);
}

Vector2d Vector2d::rotate(double theta)
{
    double s = sin(theta);
    double c = cos(theta);
    return Vector2d(x * c - y * s, x * s + y * c);
}

Vector2d operator +(Vector2d a, Vector2d b)
{
    return Vector2d(a.x + b.x, a.y + b.y);
}
Vector2d operator -(Vector2d a, Vector2d b)
{
    return Vector2d(a.x - b.x, a.y - b.y);
}
Vector2d operator -(Vector2d a)
{
    return Vector2d(-a.x, -a.y);
}
Vector2d operator *(double b, Vector2d a)
{
    return Vector2d(a.x * b, a.y * b);
}
Vector2d operator *(Vector2d a, double b)
{
    return Vector2d(a.x * b, a.y * b);
}
Vector2d operator /(Vector2d a, double b)
{
    return Vector2d(a.x / b, a.y / b);
}
double operator &(Vector2d a, Vector2d b)
{
    return a.x * b.x + a.y * b.y;
}

bool operator <(Vector2d a, Vector2d b)
{
    return a.x < b.x && a.y < b.y;
}
bool operator >=(Vector2d a, Vector2d b)
{
    return a.x >= b.x && a.y >= b.y;
}
