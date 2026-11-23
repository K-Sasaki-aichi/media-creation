#include <GL/glut.h>
#include <math.h>
#include "cuboid.h"
#include "../vector.h"

#ifndef PI
#define PI 3.14159265f
#endif

// 立方体を描く関数

#include <GL/glut.h>
#include <math.h>
#include "cuboid.h"
#include "../vector.h"

#ifndef PI
#define PI 3.14159265f
#endif

// 立方体を描く関数
void drawCuboid(float width, float height, float depth)
{
    float x = width / 2.0f;
    float y = height / 2.0f;
    float z = depth / 2.0f;

    Vec3 p1, p2, p3, p4;

    glBegin(GL_QUADS);

    // 前面 (+Z)
    p1 = (Vec3){-x, -y,  z};
    p2 = (Vec3){ x, -y,  z};
    p3 = (Vec3){ x,  y,  z};
    p4 = (Vec3){-x,  y,  z};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 背面 (-Z)
    p1 = (Vec3){ x, -y, -z};
    p2 = (Vec3){-x, -y, -z};
    p3 = (Vec3){-x,  y, -z};
    p4 = (Vec3){ x,  y, -z};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 左面 (-X)
    p1 = (Vec3){-x, -y, -z};
    p2 = (Vec3){-x, -y,  z};
    p3 = (Vec3){-x,  y,  z};
    p4 = (Vec3){-x,  y, -z};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 右面 (+X)
    p1 = (Vec3){ x, -y,  z};
    p2 = (Vec3){ x, -y, -z};
    p3 = (Vec3){ x,  y, -z};
    p4 = (Vec3){ x,  y,  z};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 上面 (+Y)
    p1 = (Vec3){-x,  y,  z};
    p2 = (Vec3){ x,  y,  z};
    p3 = (Vec3){ x,  y, -z};
    p4 = (Vec3){-x,  y, -z};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 底面 (-Y)
    p1 = (Vec3){-x, -y, -z};
    p2 = (Vec3){ x, -y, -z};
    p3 = (Vec3){ x, -y,  z};
    p4 = (Vec3){-x, -y,  z};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    glEnd();
}

/* 底面（XZ平面）を deg 度傾けた平行四辺形の四角柱を描画 */
// 左上前を原点としているので注意！！
Vec3 drawParallelogramPrism(float width, float height, float depth, float deg)
{
    const float theta = deg / 360.0f * 2.0f * PI;
    const float dx = height * cosf(theta);
    const float dy = -height * sinf(theta);

    Vec3 p1, p2, p3, p4;

    glBegin(GL_QUADS);

    // 前面 (z = 0)
    p1 = (Vec3){0.0f, 0.0f, 0.0f};
    p2 = (Vec3){dx, dy, 0.0f};
    p3 = (Vec3){dx + width, dy, 0.0f};
    p4 = (Vec3){width, 0.0f, 0.0f};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 背面 (z = depth)
    p1 = (Vec3){width, 0.0f, depth};
    p2 = (Vec3){dx + width, dy, depth};
    p3 = (Vec3){dx, dy, depth};
    p4 = (Vec3){0.0f, 0.0f, depth};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 左面
    p1 = (Vec3){0.0f, 0.0f, depth};
    p2 = (Vec3){dx, dy, depth};
    p3 = (Vec3){dx, dy, 0.0f};
    p4 = (Vec3){0.0f, 0.0f, 0.0f};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 右面
    p1 = (Vec3){dx + width, dy, depth};
    p2 = (Vec3){width, 0.0f, depth};
    p3 = (Vec3){width, 0.0f, 0.0f};
    p4 = (Vec3){dx + width, dy, 0.0f};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 上面（傾き側）
    p1 = (Vec3){dx, dy, depth};
    p2 = (Vec3){dx + width, dy, depth};
    p3 = (Vec3){dx + width, dy, 0.0f};
    p4 = (Vec3){dx, dy, 0.0f};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    // 底面（基準辺側）
    p1 = (Vec3){0.0f, 0.0f, 0.0f};
    p2 = (Vec3){width, 0.0f, 0.0f};
    p3 = (Vec3){width, 0.0f, depth};
    p4 = (Vec3){0.0f, 0.0f, depth};
    setQuadNormal(&p1, &p2, &p3, &p4);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);

    glEnd();

    return (Vec3){dx + width, dy, 0.0f};
}

// 2点間を結ぶ直方体を描画する
// 長手方向をローカルZ軸に合わせる
void drawBeam(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float thickness
)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;

    float length = sqrtf(dx*dx + dy*dy + dz*dz);

    float yaw = atan2f(dx, dz) * 180.0f / 3.14159265f;
    float horizontal = sqrtf(dx*dx + dz*dz);
    float pitch = atan2f(-dy, horizontal) * 180.0f / 3.14159265f;

    glPushMatrix();

    glTranslatef(
        (x1+x2)/2.0f,
        (y1+y2)/2.0f,
        (z1+z2)/2.0f
    );

    glRotatef(yaw, 0, 1, 0);
    glRotatef(pitch, 1, 0, 0);

    // 横幅を少し広くした押しつぶされた直方体
    drawCuboid(thickness * 1.5f, thickness, length);

    glPopMatrix();
}