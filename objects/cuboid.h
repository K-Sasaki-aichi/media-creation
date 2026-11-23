#ifndef CUBOID_H
#define CUBOID_H
#include "../vector.h"

void drawCuboid(float width, float height, float depth);
void drawBeam(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float thickness
);

Vec3 drawParallelogramPrism(float width, float height, float depth, float deg);

#endif