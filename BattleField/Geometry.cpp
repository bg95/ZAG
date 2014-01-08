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

void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks are drawn. */

    double z0,z1;
    double r0,r1;

    /* Pre-computed circle */

    double *sint1,*cost1;
    double *sint2,*cost2;


    fghCircleTable(&sint1,&cost1,-slices);
    fghCircleTable(&sint2,&cost2,stacks*2);

    /* The top stack is covered with a triangle fan */

    z0 = 1.0;
    z1 = cost2[(stacks>0)?1:0];
    r0 = 0.0;
    r1 = sint2[(stacks>0)?1:0];

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0,0,1);
        glVertex3d(0,0,radius);

        for (j=slices; j>=0; j--)
                {
                    glNormal3d(cost1[j]*r1, sint1[j]*r1, z1 );
                    glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
                }

            glEnd();

            /* Cover each stack with a quad strip, except the top and bottom stacks */

            for( i=1; i<stacks-1; i++ )
            {
                z0 = z1; z1 = cost2[i+1];
                r0 = r1; r1 = sint2[i+1];

                glBegin(GL_QUAD_STRIP);

                    for(j=0; j<=slices; j++)
                    {
                        glNormal3d(cost1[j]*r1, sint1[j]*r1, z1 );
                        glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
                        glNormal3d(cost1[j]*r0, sint1[j]*r0, z0 );
                        glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
                    }

                glEnd();
            }

            /* The bottom stack is covered with a triangle fan */

            z0 = z1;
            r0 = r1;

            glBegin(GL_TRIANGLE_FAN);
            glNormal3d(0,0,-1);
            glVertex3d(0,0,-radius);

                    for (j=0; j<=slices; j++)
                    {
                        glNormal3d(cost1[j]*r0, sint1[j]*r0, z0 );
                        glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
                    }

                glEnd();

                /* Release sin and cos tables */

                free(sint1);
                free(cost1);
                free(sint2);
                free(cost2);
}
