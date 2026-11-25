#ifndef CONTROL_STICK_H
#define CONTROL_STICK_H

#include <GL/glut.h>
#include <math.h>
#include "../vector.h"
#include "../gl_draw.h"
#include "../objects/cuboid.h"
#include "../objects/board.h"
#include "../texture/texture.h"
#include "../objects/joystick.h"
#include "../models/seat.h"

#ifndef PI
#define PI 3.14159265f
#endif

typedef enum
{
    CV_A = 0,
    CV_B,
    CV_C,
    CV_D,
    CV_E,
    CV_F,
    CV_G,
    CV_H,
    CV_I,
    CV_J,
    CV_K,
    CV_L,
    CV_M,
    CV_N,
    CV_P,
    CV_Q,
    CV_R,
    CV_COUNT
} ControlVertexId;

static const Vec3 CONTROL_VERTICES[CV_COUNT] = {
    {   0.0f, -172.0f,    0.0f }, // A
    {  58.0f, -172.0f,  605.0f }, // B
    { 272.0f, -172.0f,  605.0f }, // C
    { 330.0f, -172.0f,    0.0f }, // D
    { 330.0f,    0.0f,    0.0f }, // E
    {   0.0f,    0.0f,    0.0f }, // F
    {   0.0f,   33.0f,  512.0f }, // G
    { 330.0f,   33.0f,  512.0f }, // H
    {   0.0f,    0.0f,    0.0f }, // I: 計算
    {   0.0f,    0.0f,    0.0f }, // J: 計算
    {   0.0f,    0.0f,    0.0f }, // K: 計算
    {   0.0f,    0.0f,    0.0f }, // L: 計算
    { -95.25f, -176.45f, 779.18f }, // M
    {   0.0f,    0.0f,    0.0f }, // N: 計算
    { -95.25f, -176.45f, 1060.0f }, // P
    {-147.5f, -125.0f, 1060.0f },  // Q
    { 477.5f, -125.0f, 1060.0f }  // Q
};

/* キャノピー下部に沿った方向ベクトルと基準・オフセットベクトル */
static const Vec3 loweFrameDir = { -147.5f, -125.0f, 1060.0f };

static const Vec3 VEC_G   = {   0.0f,   33.0f, 512.0f };
static const Vec3 DG_TO_I = { -79.0f, -101.0f,  62.0f }; /* G -> I */
static const Vec3 DG_TO_J = { -21.9f, -145.5f, 173.0f }; /* G -> J */

static void drawSideWall(float height, const Vec3* v)
{
    Vec3 dir1 = sub(v[CV_P], v[CV_M]);
    Vec3 dir2 = sub(v[CV_A], v[CV_N]);

    Vec3 origin = { 0.0f, 0.0f, 0.0f };
    Vec3 a = sub(v[CV_A], v[CV_N]);
    Vec3 b = sub(v[CV_B], v[CV_N]);

    glPushMatrix();
    glTranslatef3v(&v[CV_N]);

    /* 左側の板 */
    drawSlantedPlate(dir1, height);

    /* 右側の板 */
    drawSlantedPlate(dir2, height);

    /* 底面 */
    glBegin(GL_TRIANGLES);
    drawTriangleV(&origin, &b, &a);
    glEnd();

    glPopMatrix();
}

static void drawWall(float height, float s, const Vec3* v)
{
    const float mirrorCenterX = v[CV_D].x / 2.0f;

    glPushMatrix();
    glScalef(s, s, s);

    /* 左側 */
    drawSideWall(height, v);

    /* 右側（左右反転） */
    glPushMatrix();
    glFrontFace(GL_CW);

    glTranslatef(mirrorCenterX, 0.0f, 0.0f);
    glScalef(-1.0f, 1.0f, 1.0f);
    glTranslatef(-mirrorCenterX, 0.0f, 0.0f);

    drawSideWall(height, v);

    glFrontFace(GL_CCW);
    glPopMatrix();

    /* 前面の板 */
    Vec3 dir = sub(v[CV_A], v[CV_D]);
    glPushMatrix();
    glTranslatef3v(&v[CV_D]);
    drawSlantedPlate(dir, height - 75);
    glPopMatrix();

    /* 後面の板 */    
    dir = sub(v[CV_R], v[CV_Q]);
    glPushMatrix();
    glTranslatef3v(&v[CV_Q]);
    drawSlantedPlate(dir, height - 130);
    glPopMatrix();

    glPopMatrix();
}

static Vec3 drawControlColumn(float s, const Vec3* v)
{
    Vec3 m = v[CV_M];
    Vec3 l = v[CV_L];

    /* MからLへのベクトル */
    Vec3 dir = sub(l, m);

    /* ML間の距離 */
    float len = length(dir);

    /* XY平面上の回転角 */
    float angle = atan2f(dir.y, dir.x) * 180.0f / PI;

    /* Z方向の長さ */
    float depth = 1060.0f - m.z;

    glPushMatrix();
    glTranslatef(m.x * s, m.y * s, m.z * s);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    /* 平行六面体を描画 */
    glScalef(s, s, s);
    Vec3 point = drawParallelogramPrism(len, len, depth, 70.0f);

    glPopMatrix();

    /* 回転とスケールを反映してワールド座標に戻す */
    point = rotateZ(point, angle);
    point = scale(point, s);

    return add(point, m);
}

/* scale : オブジェクトの縮尺 */
/* scale : オブジェクトの縮尺 */
static void drawControlside(float scale, Vec3* v)
{
    glPushMatrix();
    glScalef(scale, scale, scale);

    v[CV_N] = drawControlColumn(1.0f, v);

    /* 上部 */
    glBegin(GL_TRIANGLES);

    drawTriangleV(&v[CV_F], &v[CV_I], &v[CV_G]);

    glEnd();

    /* 上部の一部（I -> K -> J）に texid_sideout1 を貼付 */
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid_sideout1);

    glBegin(GL_TRIANGLES);

    drawTexturedTriangleUV(
        &v[CV_I], 0.0f,         1.0f,
        &v[CV_K], 145.0f/309.0f, 0.0f,
        &v[CV_J], 1.0f,         1.0f
    );

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    /* 上部の一部（I -> J -> G）に texid_sideout0 を貼付 */
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid_sideout0);

    glBegin(GL_TRIANGLES);

    drawTexturedTriangleUV(
        &v[CV_I], 0.0f,         1.0f,
        &v[CV_J], 1.0f,         1.0f,
        &v[CV_G], 76.0f/256.0f, 0.0f
    );

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    /* side in */
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid_sidein);

    glBegin(GL_TRIANGLES);

    drawTexturedTriangleUV(
        &v[CV_G], 0.0f,          0.0f,
        &v[CV_L], 540.0f/590.0f, 373.0f/526.0f,
        &v[CV_B], 96.0f/590.0f,  1.0f
    );

    drawTexturedTriangleUV(
        &v[CV_L], 540.0f/590.0f, 373.0f/526.0f,
        &v[CV_N], 1.0f,          1.0f,
        &v[CV_B], 96.0f/590.0f,  1.0f
    );

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    /* 下部（K -> M -> L, K -> J -> L）に texid_sideout2 を貼付 */
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid_sideout2);

    glBegin(GL_TRIANGLES);

    /* 三角形1: K(0,0) -> M(1,0) -> L(1,1) */
    drawTexturedTriangleUV(
        &v[CV_K], 1.0f, 0.0f,
        &v[CV_M], 1.0f, 1.0f,
        &v[CV_L], 0.0f, 1.0f
    );

    /* 三角形2: K(0,0) -> J(0,1) -> L(1,1) */
    drawTexturedTriangleUV(
        &v[CV_K], 1.0f, 0.0f,
        &v[CV_J], 0.0f, 0.0f,
        &v[CV_L], 0.0f, 1.0f
    );

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    /* 側面 */
    glBegin(GL_TRIANGLES);
    glColor3f(0.443f, 0.447f, 0.443f);
    //drawQuadV(&v[CV_M], &v[CV_I], &v[CV_Q], &v[CV_P]);
    drawTriangleV(&v[CV_M], &v[CV_I], &v[CV_Q]);
    drawTriangleV(&v[CV_M], &v[CV_Q], &v[CV_P]);

    glEnd();

    glPopMatrix();
}

static void drawControlPanel(const Vec3* v)
{
    Vec3 p1 = v[CV_G];
    Vec3 p2 = add(withX(v[CV_B], p1.x), (Vec3){-8.0f, 0, 0});

    Vec3 p4 = v[CV_H];
    Vec3 p3 = withX(v[CV_C], p4.x);
    

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid_control_center);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3v(&p1);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3v(&p2);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3v(&p3);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3v(&p4);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

/* ワールド座標での原点位置を指定して描画 */
/* scale : オブジェクトの縮尺 */
/* ワールド座標での原点位置を指定して描画 */
/* scale : オブジェクトの縮尺 */
static void drawControlCenter(float scale, const Vec3* v)
{
    glPushMatrix();
    glScalef(scale, scale, scale);

    /* ===================================== */
    /* 底面・前面                            */
    /* ===================================== */
    glBegin(GL_QUADS);

    /* 底面: A -> D -> C -> B */
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3v(&v[CV_A]);
    glVertex3v(&v[CV_D]);
    glVertex3v(&v[CV_C]);
    glVertex3v(&v[CV_B]);

    /* 前面: A -> F -> E -> D */
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3v(&v[CV_A]);
    glVertex3v(&v[CV_F]);
    glVertex3v(&v[CV_E]);
    glVertex3v(&v[CV_D]);

    glEnd();

    /* ===================================== */
    /* 上面: F -> G -> H -> E（テクスチャ貼付） */
    /* ===================================== */
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid_plate);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.9979f, -0.0644f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3v(&v[CV_F]);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3v(&v[CV_G]);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3v(&v[CV_H]);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3v(&v[CV_E]);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    /* ===================================== */
    /* 左右側面                              */
    /* ===================================== */
    glBegin(GL_TRIANGLES);

    /* 左側面1 */
    glNormal3f(-0.981f, -0.177f, 0.071f);
    glVertex3v(&v[CV_A]);
    glVertex3v(&v[CV_B]);
    glVertex3v(&v[CV_G]);

    /* 左側面2 */
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3v(&v[CV_A]);
    glVertex3v(&v[CV_G]);
    glVertex3v(&v[CV_F]);

    /* 右側面1 */
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3v(&v[CV_D]);
    glVertex3v(&v[CV_E]);
    glVertex3v(&v[CV_H]);

    /* 右側面2 */
    glNormal3f(0.981f, -0.177f, 0.071f);
    glVertex3v(&v[CV_D]);
    glVertex3v(&v[CV_H]);
    glVertex3v(&v[CV_C]);

    glEnd();

    drawControlPanel(v);

    glPopMatrix();
}

/* コックピット下部に床板を描画 */
static void drawFloor(float floorY, float s)
{
    Vec3 p1 = { -250.0f, floorY, -100.0f };
    Vec3 p2 = { -250.0f, floorY, 1400.0f };
    Vec3 p3 = {  580.0f, floorY, 1400.0f };
    Vec3 p4 = {  580.0f, floorY, -100.0f };

    glPushMatrix();
    glScalef(s, s, s);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3v(&p1);
    glVertex3v(&p2);
    glVertex3v(&p3);
    glVertex3v(&p4);
    glEnd();

    glPopMatrix();
}

static void initControlVertices(Vec3 out[CV_COUNT])
{
    int i;

    for (i = 0; i < CV_COUNT; i++)
    {
        out[i] = CONTROL_VERTICES[i];
    }

    out[CV_I] = add(out[CV_G], DG_TO_I);
    out[CV_J] = add(out[CV_G], DG_TO_J);

    out[CV_K] = withY(
        add(out[CV_I], scale(loweFrameDir, 0.125f)),
        out[CV_J].y
    );

    out[CV_L] = withZ(
        sub(scale(out[CV_J], 1.5f),scale(out[CV_G], 0.5f)),
        out[CV_M].z
    );
}

static void drawCockpit(float s){
    Vec3 v[CV_COUNT];
    initControlVertices(v);

    const float mirrorCenterX = 165.0f; /* 0.0f と 330.0f の中点 */


    glColor3f(0.7,0.7,0.7);
    drawCanopy(s);

    // 操縦桿の中央部分
    drawControlCenter(s, v);

    // 左側の側面
    drawControlside(s, v);

    // 右側の側面（x = 165.0f を中心にX反転＋平行移動）
    glPushMatrix();
    glFrontFace(GL_CW);
    glTranslatef(mirrorCenterX * s, 0.0f, 0.0f);
    glScalef(-1.0f, 1.0f, 1.0f);
    glTranslatef(-mirrorCenterX * s, 0.0f, 0.0f);
    drawControlside(s, v);
    glFrontFace(GL_CCW);
    glPopMatrix();

    glColor3f(0.153f, 0.153f, 0.149f);
    drawWall(-70, s, v);

    drawFloor(-316.89f, s);

    /* 操縦桿 */
    glPushMatrix();
    glScalef(s, s, s);
    glTranslatef(165.0f, -300.0f, 600.0f);
    drawControlStick();
    glPopMatrix();

    /* 椅子 */
    glPushMatrix();
    glScalef(s, s, s);
    glTranslatef(165.0f, 0.0f, 1000.0f);
    drawSeat();
    glPopMatrix();
}

#endif