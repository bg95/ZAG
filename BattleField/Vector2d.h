#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2d
{
public:
    Vector2d();
    Vector2d(double _x, double _y);
    Vector2d &operator =(Vector2d b);
    double abs();

    double c[2];
    double &x = c[0], &y = c[1];
};

Vector2d operator +(Vector2d a, Vector2d b);
Vector2d operator -(Vector2d a, Vector2d b);
Vector2d operator -(Vector2d a);
Vector2d operator *(double b, Vector2d a);
Vector2d operator *(Vector2d a, double b);
Vector2d operator /(Vector2d a, double b);
double operator &(Vector2d a, Vector2d b);
//the following are used in QuardTree
bool operator <(Vector2d a, Vector2d b);
bool operator >=(Vector2d a, Vector2d b);

#endif // VECTOR2D_H
