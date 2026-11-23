#ifndef DRAW_H
#define DRAW_H

#include "vector.h"
#include <GL/glut.h>

static void drawTriangleV(const Vec3* p1, const Vec3* p2, const Vec3* p3)
{
    setTriangleNormal(p1, p2, p3);
    glVertex3v(p1);
    glVertex3v(p2);
    glVertex3v(p3);
}

#endif