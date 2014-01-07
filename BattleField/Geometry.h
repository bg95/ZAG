#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/gl.h>

static void fghCircleTable(double **, double **, const int );
void glutSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks);
void drawBattleField();
#endif // GEOMETRY_H
