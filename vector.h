#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x, y, z;
} Vector3;

Vector3 add(Vector3 a, Vector3 b);
Vector3 sub(Vector3 a, Vector3 b);
Vector3 scale(Vector3 v, float s);
float dot(Vector3 a, Vector3 b);
Vector3 cross(Vector3 a, Vector3 b);
float length(Vector3 v);
Vector3 normalize(Vector3 v);

#endif