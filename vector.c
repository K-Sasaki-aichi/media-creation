#include <math.h>
#include <GL/glut.h>
#include "vector.h"

Vector3 add(Vector3 a, Vector3 b) {
    return (Vector3){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

Vector3 sub(Vector3 a, Vector3 b) {
    return (Vector3){
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

Vector3 scale(Vector3 v, float s) {
    return (Vector3){
        v.x * s,
        v.y * s,
        v.z * s
    };
}

// 内積
float dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 外積
Vector3 cross(Vector3 a, Vector3 b) {
    return (Vector3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float length(Vector3 v) {
    return sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );
}

Vector3 normalize(Vector3 v) {
    float len = length(v);

    if (len == 0.0f) return (Vector3){0, 0, 0};

    return scale(v, 1.0f / len);
}

void glVertexVector3(Vector3 v) {
    GLdouble pos[] = {v.x, v.y, v.z};
    glVertex3dv(pos);
}