#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glut.h>

// 球体を描画する
void drawSphere(float radius, int slices, int stacks)
{
    GLUquadric *quad = gluNewQuadric();

    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);

    gluSphere(quad, radius, slices, stacks);

    gluDeleteQuadric(quad);
}

#endif