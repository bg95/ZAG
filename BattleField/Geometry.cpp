#include <QPainter>
#include <math.h>
#include <GL/gl.h>
#include "../main.h"
#include "BattleField.h"

static void fghCircleTable(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */

    const double angle = 2*PI/(double)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

    /* Bail out if memory allocation fails, fgError never returns */

    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
    }

    /* Compute cos and sin around the circle */

    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
            (*sint)[i] = sin(angle*i);
            (*cost)[i] = cos(angle*i);
        }

        /* Last sample is duplicate of the first */

        (*sint)[size] = (*sint)[0];
        (*cost)[size] = (*cost)[0];
}

void glutSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks) {
        int i,j;

        /* Step in z and radius as stacks are drawn. */

        double z0,z1;
        const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

        /* Pre-computed circle */

        double *sint,*cost;

        fghCircleTable(&sint,&cost,-slices);
        /* Cover the base and top */

            glBegin(GL_TRIANGLE_FAN);
                glNormal3d(0.0, 0.0, -1.0 );
                glVertex3d(0.0, 0.0, 0.0 );
                for (j=0; j<=slices; j++)
                  glVertex3d(cost[j]*radius, sint[j]*radius, 0.0);
            glEnd();

            glBegin(GL_TRIANGLE_FAN);
                glNormal3d(0.0, 0.0, 1.0 );
                glVertex3d(0.0, 0.0, height);
                for (j=slices; j>=0; j--)
                  glVertex3d(cost[j]*radius, sint[j]*radius, height);
            glEnd();

            /* Do the stacks */

            z0 = 0.0;
            z1 = zStep;

            for (i=1; i<=stacks; i++)
            {
                if (i==stacks)
                    z1 = height;

                glBegin(GL_QUAD_STRIP);
                    for (j=0; j<=slices; j++ )
                    {
                        glNormal3d(cost[j], sint[j], 0.0 );
                        glVertex3d(cost[j]*radius, sint[j]*radius, z0 );
                        glVertex3d(cost[j]*radius, sint[j]*radius, z1 );
                    }
                                glEnd();

                                z0 = z1; z1 += zStep;
                            }

                            /* Release sin and cos tables */

                            free(sint);
                            free(cost);
}

void drawBattleField() {
    int numBar = 35;
    double step = 20.0 / (double)numBar;
    double radius = step / 20;
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPushMatrix();
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glTranslatef(-10.0, 0.0, -10.0);
        for(double x = 0; x <= 20.0 + step; x += step) {
            for(double z = 0; z < 20.0; z += step) {
                glutSolidCylinder(radius, step, 6, 10);
                glTranslated(0.0, 0.0, step);
            }
            glTranslated(0.0, 0.0, -20.0);
            glTranslated(step, 0.0, 0.0);
        }
    glPopMatrix();
    glPushMatrix();
        glRotatef(90.0, 0.0, 1.0, 0.0);
        glTranslatef(0.0, -10.0, -10.0);
        for(double y = 0; y <= 20.0 + step; y += step) {
            glutSolidCylinder(radius, 20.0, 6, 10);
            glTranslated(0.0, step, 0.0);
        }
    glPopMatrix();
}
