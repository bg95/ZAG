#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/gl.h>

static void fghCircleTable(double **, double **, const int );
void glutSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks);
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);
#endif // GEOMETRY_H
