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

static void beginGlass(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // 少し曇った青みがかったガラス
    glColor4f(0.35f, 0.65f, 0.78f, 0.30f);
}

static void endGlass(void)
{
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

#endif