#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/gl.h>

/*
 * some basic opengl operations:
 * draw cylinder, draw sphere
 * NOTE: these codes are copied from github.com because no relavant functions are provided in Qt Creator
 */


static void fghCircleTable(double **, double **, const int );
void glutSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks);
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);
#endif // GEOMETRY_H
