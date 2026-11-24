#ifndef DRAW_H
#define DRAW_H

#include "vector.h"

static void drawTriangleV(const Vec3* p1, const Vec3* p2, const Vec3* p3)
{
    setTriangleNormal(p1, p2, p3);
    glVertex3v(p1);
    glVertex3v(p2);
    glVertex3v(p3);
}

static void drawTexturedTriangleUV(
    const Vec3* p1, float u1, float v1,
    const Vec3* p2, float u2, float v2,
    const Vec3* p3, float u3, float v3
)
{
    setTriangleNormal(p1, p2, p3);

    glTexCoord2f(u1, v1);
    glVertex3v(p1);

    glTexCoord2f(u2, v2);
    glVertex3v(p2);

    glTexCoord2f(u3, v3);
    glVertex3v(p3);
}

static void drawQuadV(const Vec3* p1, const Vec3* p2, const Vec3* p3, const Vec3* p4)
{
    drawTriangleV(p1, p2, p3);
    drawTriangleV(p1, p3, p4);
}

static void drawSlantedPlate(const Vec3 dir, float bottom)
{
    Vec3 p1 = {0.0f, 0.0f, 0.0f};
    Vec3 p2 = dir;
    Vec3 p3 = withY(p2, bottom);
    Vec3 p4 = {0.0f, bottom, 0.0f};

    glBegin(GL_QUADS);
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);
    glEnd();
}

#endif