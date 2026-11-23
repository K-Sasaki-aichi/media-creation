#ifndef CONTROL_STICK_H
#define CONTROL_STICK_H

#include <GL/glut.h>
#include <math.h>
#include "../vector.h"
#include "../gl_draw.h"

typedef enum
{
    V_A = 0,
    V_B,
    V_C,
    V_D,
    V_E,
    V_F,
    V_G,
    V_H,
    V_CENTER_COUNT
} CenterVertexId;

typedef enum
{
    SV_F = 0,
    SV_G,
    SV_I,
    SV_K,
    SV_J,
    SV_B,
    SV_L,
    SV_M,
    SV_COUNT
} SideVertexId;

/* A～H の頂点座標テーブル */
static const Vec3 CONTROL_CENTER_VERTICES[V_CENTER_COUNT] = {
    {   0.0f, -172.0f,   0.0f }, /* A: 底面・左前 */
    {  44.0f, -172.0f, 605.0f }, /* B: 底面・左後 */
    { 286.0f, -172.0f, 605.0f }, /* C: 底面・右後 */
    { 330.0f, -172.0f,   0.0f }, /* D: 底面・右前 */
    { 330.0f,    0.0f,   0.0f }, /* E: 前面上端・右 */
    {   0.0f,    0.0f,   0.0f }, /* F: 前面上端・左 */
    {   0.0f,   33.0f, 512.0f }, /* G: 上面奥辺・左 */
    { 330.0f,   33.0f, 512.0f }  /* H: 上面奥辺・右 */
};

/* キャノピー下部に沿った方向ベクトルと基準・オフセットベクトル */
static const Vec3 loweFrameDir = { -147.5f, -125.0f, 1060.0f };

static const Vec3 VEC_G   = {   0.0f,   33.0f, 512.0f };
static const Vec3 DG_TO_I = { -79.0f, -101.0f,  62.0f }; /* G -> I */
static const Vec3 DG_TO_J = { -21.9f, -145.5f, 173.0f }; /* G -> J */

/* F, G, I, K, J, B のベース座標テーブル（I, K, J は initControlSideVertices で計算） */
static const Vec3 CONTROL_SIDE_BASE_VERTICES[SV_COUNT] = {
    {   0.0f,    0.0f,   0.0f }, /* F: 前面上端・左 */
    {   0.0f,   33.0f, 512.0f }, /* G: 上面奥辺・左 */
    {   0.0f,    0.0f,   0.0f }, /* I: G + DG_TO_I */
    {   0.0f,    0.0f,   0.0f }, /* K: I + calcDiToK() */
    {   0.0f,    0.0f,   0.0f }, /* J: G + DG_TO_J */
    {  44.0f, -172.0f, 605.0f }, /* B: 底面・左後 */
    {   0.0f,    0.0f,   0.0f }, /* H: (J-G)*3/2 + G */
    {   0.0f,    0.0f,   0.0f }  /* H: (J-G)*3/2 + G */
};

/* CONTROL_SIDE 用の頂点配列を組み立てる */
static void initControlSideVertices(Vec3 out[SV_COUNT])
{
    int i;
    for (i = 0; i < SV_COUNT; i++)
    {
        out[i] = CONTROL_SIDE_BASE_VERTICES[i];
    }
    out[SV_I] = add(VEC_G, DG_TO_I);
    out[SV_J] = add(VEC_G, DG_TO_J);
    out[SV_K] = withY(add(out[SV_I], scale(loweFrameDir, 0.125f)), out[SV_J].y);
    out[SV_L] = sub(scale(out[SV_J], 1.5f), scale(out[SV_G], 0.5f));
    out[SV_M] = withX(out[SV_L], out[SV_K].x);
}

/* F, G, I, K, J, B を通る側面図形を描画 */
/* scale : オブジェクトの縮尺 */
static void drawControlside(float scale)
{
    Vec3 v[SV_COUNT];
    initControlSideVertices(v);

    glPushMatrix();
    glScalef(scale, scale, scale);

    /* 側面図形を三角形分割して描画 */
    glBegin(GL_TRIANGLES);

    /* F -> G -> I */
    drawTriangleV(&v[SV_F], &v[SV_G], &v[SV_I]);

    /* I -> J -> G */
    drawTriangleV(&v[SV_I], &v[SV_J], &v[SV_G]);

    /* I -> J -> K */
    drawTriangleV(&v[SV_I], &v[SV_J], &v[SV_K]);

    /* G -> L -> B */
    drawTriangleV(&v[SV_G], &v[SV_L], &v[SV_B]);

    /* K -> J -> L -> M */
    drawTriangleV(&v[SV_K], &v[SV_J], &v[SV_L]);
    drawTriangleV(&v[SV_K], &v[SV_L], &v[SV_M]);
    
    glEnd();

    glPopMatrix();
}

/* ワールド座標での原点位置を指定して描画 */
/* scale : オブジェクトの縮尺 */
static void drawControlCenter(float scale)
{
    const Vec3* v = CONTROL_CENTER_VERTICES;

    glPushMatrix();
    glScalef(scale, scale, scale);

    /* ===================================== */
    /* A～Hの頂点で構成する立体               */
    /* 底面・前面・上面・後面                */
    /* ===================================== */
    glBegin(GL_QUADS);

    /* 底面: A -> D -> C -> B */
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3v(&v[V_A]);
    glVertex3v(&v[V_D]);
    glVertex3v(&v[V_C]);
    glVertex3v(&v[V_B]);

    /* 前面: A -> F -> E -> D */
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3v(&v[V_A]);
    glVertex3v(&v[V_F]);
    glVertex3v(&v[V_E]);
    glVertex3v(&v[V_D]);

    /* 上面: F -> G -> H -> E */
    glNormal3f(0.0f, 0.9979f, -0.0644f);
    glVertex3v(&v[V_F]);
    glVertex3v(&v[V_G]);
    glVertex3v(&v[V_H]);
    glVertex3v(&v[V_E]);

    /* 後面: B -> C -> H -> G */
    glNormal3f(0.0f, 0.414f, 0.910f);
    glVertex3v(&v[V_B]);
    glVertex3v(&v[V_C]);
    glVertex3v(&v[V_H]);
    glVertex3v(&v[V_G]);

    glEnd();

    /* ===================================== */
    /* 左右側面                              */
    /* 非平面の四角形なので三角形に分割      */
    /* ===================================== */
    glBegin(GL_TRIANGLES);

    /* 左側面 1: A -> B -> G */
    glNormal3f(-0.981f, -0.177f, 0.071f);
    glVertex3v(&v[V_A]);
    glVertex3v(&v[V_B]);
    glVertex3v(&v[V_G]);

    /* 左側面 2: A -> G -> F */
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3v(&v[V_A]);
    glVertex3v(&v[V_G]);
    glVertex3v(&v[V_F]);

    /* 右側面 1: D -> E -> H */
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3v(&v[V_D]);
    glVertex3v(&v[V_E]);
    glVertex3v(&v[V_H]);

    /* 右側面 2: D -> H -> C */
    glNormal3f(0.981f, -0.177f, 0.071f);
    glVertex3v(&v[V_D]);
    glVertex3v(&v[V_H]);
    glVertex3v(&v[V_C]);

    glEnd();

    glPopMatrix();
}

#endif