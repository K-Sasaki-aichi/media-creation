
#ifndef BOX_FRAME_H
#define BOX_FRAME_H

#include <GL/glut.h>
#include "cuboid.h"

// 直方体を骨組みで囲む
// width  : X方向の長さ
// height : Y方向の長さ
// depth  : Z方向の長さ
// t      : 骨組みの太さ
void drawBoxFrame(float width, float height, float depth, float t)
{
    float x = width  / 2.0f;
    float y = height / 2.0f;
    float z = depth  / 2.0f;

    // -------------------------
    // 下側の四辺
    // -------------------------

    drawBeam(-x, -y, -z,  x, -y, -z, t);
    drawBeam( x, -y, -z,  x, -y,  z, t);
    drawBeam( x, -y,  z, -x, -y,  z, t);
    drawBeam(-x, -y,  z, -x, -y, -z, t);

    // -------------------------
    // 上側の四辺
    // -------------------------

    drawBeam(-x,  y, -z,  x,  y, -z, t);
    drawBeam( x,  y, -z,  x,  y,  z, t);
    drawBeam( x,  y,  z, -x,  y,  z, t);
    drawBeam(-x,  y,  z, -x,  y, -z, t);

    // -------------------------
    // 上下をつなぐ四本
    // -------------------------

    drawBeam(-x, -y, -z, -x,  y, -z, t);
    drawBeam( x, -y, -z,  x,  y, -z, t);
    drawBeam( x, -y,  z,  x,  y,  z, t);
    drawBeam(-x, -y,  z, -x,  y,  z, t);
}

#endif
