#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2d
{
public:
    Vector2d();
    Vector2d(double _x, double _y);
    double abs();

    union
    {
        struct
        {
            double x, y;
        };
        double c[2];
    };
};

Vector2d operator +(Vector2d a, Vector2d b);
Vector2d operator -(Vector2d a, Vector2d b);
Vector2d operator -(Vector2d a);

#endif // VECTOR2D_H
