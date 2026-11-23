#ifndef VECTOR_H
#define VECTOR_H

#include <GL/glut.h>
#include <math.h>

typedef struct
{
    float x;
    float y;
    float z;
} Vec3;

Vec3 add(Vec3 a, Vec3 b) {
    return (Vec3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

Vec3 sub(Vec3 a, Vec3 b) {
    return (Vec3){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

Vec3 scale(Vec3 v, float s) {
    return (Vec3){
        v.x * s,
        v.y * s,
        v.z * s
    };
}

// 内積
float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 外積
Vec3 cross(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float length(Vec3 v) {
    return sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );
}

Vec3 normalize(Vec3 v) {
    float len = length(v);

    if (len == 0.0f) return (Vec3){0, 0, 0};

    return scale(v, 1.0f / len);
}

void glVertexVec3(Vec3 v) {
    GLdouble pos[] = {v.x, v.y, v.z};
    glVertex3dv(pos);
}

static inline void glVertex3v(const Vec3* v)
{
    glVertex3f(v->x, v->y, v->z);
}

static inline void glNormal3v(const Vec3* n)
{
    glNormal3f(n->x, n->y, n->z);
}

/* 三角形 p1 -> p2 -> p3 の単位法線ベクトルを計算して設定 */
static void setTriangleNormal(const Vec3* p1, const Vec3* p2, const Vec3* p3)
{
    float ux = p2->x - p1->x;
    float uy = p2->y - p1->y;
    float uz = p2->z - p1->z;

    float vx = p3->x - p1->x;
    float vy = p3->y - p1->y;
    float vz = p3->z - p1->z;

    float nx = uy * vz - uz * vy;
    float ny = uz * vx - ux * vz;
    float nz = ux * vy - uy * vx;

    float len = sqrtf(nx * nx + ny * ny + nz * nz);
    if (len > 1e-6f)
    {
        nx /= len;
        ny /= len;
        nz /= len;
    }

    glNormal3f(nx, ny, nz);
}

#endif