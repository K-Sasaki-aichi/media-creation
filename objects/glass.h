#ifndef GLASS_H
#define GLASS_H

#include <GL/glut.h>
#include "../vector.h"

// ガラスを定義する
static void drawGlassQuad(
    const Vec3* p1, const Vec3* p2, const Vec3* p3, const Vec3* p4,
    float edge_alpha, float center_alpha
)
{
    Vec3 center;
    const float r = 0.75f;
    const float g = 0.88f;
    const float b = 0.96f;

    center.x = 0.25f * (p1->x + p2->x + p3->x + p4->x);
    center.y = 0.25f * (p1->y + p2->y + p3->y + p4->y);
    center.z = 0.25f * (p1->z + p2->z + p3->z + p4->z);

    setQuadNormal(p1, p2, p3, p4);

    glBegin(GL_TRIANGLE_FAN);
        glColor4f(r, g, b, center_alpha);
        glVertex3f(center.x, center.y, center.z);

        glColor4f(r, g, b, edge_alpha);
        glVertex3v(p1);
        glVertex3v(p2);
        glVertex3v(p3);
        glVertex3v(p4);
        glVertex3v(p1);
    glEnd();
}

#endif