#ifndef TYPES_H
#define TYPES_H

#include <GL/glut.h>

// ################################
// 3次元ベクトル
// ################################
typedef struct {
    GLdouble x;
    GLdouble y;
    GLdouble z;
} Vector3;

Vector3 add(Vector3 a, Vector3 b);
Vector3 sub(Vector3 a, Vector3 b);
Vector3 scale(Vector3 v, float s);
float dot(Vector3 a, Vector3 b);
Vector3 cross(Vector3 a, Vector3 b);
float length(Vector3 v);
Vector3 normalize(Vector3 v);

// ################################
// 色
// ################################
typedef struct {
    GLdouble r;
    GLdouble g;
    GLdouble b;
    GLdouble a;
} Color;

// ################################
// マテリアル
// ################################
typedef struct {
    Color ambient;
    Color diffuse;
    Color specular;
    GLdouble shininess;
    GLuint textureID;
} Material;

#endif