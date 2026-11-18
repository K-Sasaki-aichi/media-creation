#include <GL/glut.h>
#include "cuboid.h"
#include <math.h>

// 立方体を描く関数
void drawCuboid(float width, float height, float depth)
{
    float x = width / 2.0f;
    float y = height / 2.0f;
    float z = depth / 2.0f;

    glBegin(GL_QUADS);

    // 前面 (+Z)
    glVertex3f(-x, -y,  z);
    glVertex3f( x, -y,  z);
    glVertex3f( x,  y,  z);
    glVertex3f(-x,  y,  z);

    // 背面 (-Z)
    glVertex3f( x, -y, -z);
    glVertex3f(-x, -y, -z);
    glVertex3f(-x,  y, -z);
    glVertex3f( x,  y, -z);

    // 左面 (-X)
    glVertex3f(-x, -y, -z);
    glVertex3f(-x, -y,  z);
    glVertex3f(-x,  y,  z);
    glVertex3f(-x,  y, -z);

    // 右面 (+X)
    glVertex3f( x, -y,  z);
    glVertex3f( x, -y, -z);
    glVertex3f( x,  y, -z);
    glVertex3f( x,  y,  z);

    // 上面 (+Y)
    glVertex3f(-x,  y,  z);
    glVertex3f( x,  y,  z);
    glVertex3f( x,  y, -z);
    glVertex3f(-x,  y, -z);

    // 底面 (-Y)
    glVertex3f(-x, -y, -z);
    glVertex3f( x, -y, -z);
    glVertex3f( x, -y,  z);
    glVertex3f(-x, -y,  z);

    glEnd();
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