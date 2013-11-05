#include <cmath>
#include "Vector2d.h"

Vector2d::Vector2d()
{
}

Vector2d::Vector2d(double _x, double _y)
    :x(_x), y(_y)
{
}

double Vector2d::abs()
{
    return sqrt(x * x + y * y);
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
    return !(a < b);
}
