#ifndef VECTOR_H
#define VECTOR_H

#include <GL/glut.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265f
#endif

typedef struct
{
    float x;
    float y;
    float z;
} Vec3;

static Vec3 add(Vec3 a, Vec3 b) {
    return (Vec3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

static Vec3 sub(Vec3 a, Vec3 b) {
    return (Vec3){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

static Vec3 scale(Vec3 v, float s) {
    return (Vec3){
        v.x * s,
        v.y * s,
        v.z * s
    };
}

// 内積
static float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 外積
static Vec3 cross(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static float length(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static float length2(Vec3 v1, Vec3 v2) {
    return length(sub(v1, v2));
}

/* ローカル座標をZ軸回りに回転 */
static Vec3 rotateZ(Vec3 p, float angle)
{
    float rad = angle * PI / 180.0f;
    float c = cosf(rad);
    float s = sinf(rad);

    return (Vec3){
        p.x * c - p.y * s,
        p.x * s + p.y * c,
        p.z
    };
}

static Vec3 normalize(Vec3 v) {
    float len = length(v);

    if (len == 0.0f) return (Vec3){0, 0, 0};

    return scale(v, 1.0f / len);
}

static Vec3 withX(Vec3 v, float x)
{
    return (Vec3){ x, v.y, v.z };
}

static Vec3 withY(Vec3 v, float y)
{
    return (Vec3){ v.x, y, v.z };
}

static Vec3 withZ(Vec3 v, float z)
{
    return (Vec3){ v.x, v.y, z };
}

static void glVertexVec3(Vec3 v) {
    GLdouble pos[] = {v.x, v.y, v.z};
    glVertex3dv(pos);
}

static inline void glVertex3v(const Vec3* v)
{
    glVertex3f(v->x, v->y, v->z);
}

static void glTranslatef3v(const Vec3* v)
{
    glTranslatef(v->x, v->y, v->z);
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

/* 四角形 p1 -> p2 -> p3 -> p4 の法線ベクトルを計算して設定 */
static void setQuadNormal(const Vec3* p1, const Vec3* p2, const Vec3* p3, const Vec3* p4)
{
    /* 対角線1: p1 -> p3, 対角線2: p2 -> p4 */
    float ux = p3->x - p1->x;
    float uy = p3->y - p1->y;
    float uz = p3->z - p1->z;

    float vx = p4->x - p2->x;
    float vy = p4->y - p2->y;
    float vz = p4->z - p2->z;

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