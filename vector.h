#ifndef VECTOR_H
#define VECTOR_H

#include <GL/glut.h>

typedef struct
{
    float x;
    float y;
    float z;
} Vec3;

static inline void glVertex3v(const Vec3* v)
{
    glVertex3f(v->x, v->y, v->z);
}

static inline void glNormal3v(const Vec3* n)
{
    glNormal3f(n->x, n->y, n->z);
}

#endif