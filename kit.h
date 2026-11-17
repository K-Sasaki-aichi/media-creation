#ifndef KIT_H
#define KIT_H

#include <GL/glut.h>

void drawCuboid(float width, float height, float depth);
void drawBeam(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float thickness
);

#endif