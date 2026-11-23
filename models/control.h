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
    V_I,
    V_J,
    V_COUNT
} VertexId;

/* A～J の頂点座標テーブル */
static const Vec3 CONTROL_CENTER_VERTICES[V_COUNT] = {
    {   0.0f, -172.0f,   0.0f }, /* A: 底面・左前 */
    {  44.0f, -172.0f, 605.0f }, /* B: 底面・左後 */
    { 286.0f, -172.0f, 605.0f }, /* C: 底面・右後 */
    { 330.0f, -172.0f,   0.0f }, /* D: 底面・右前 */
    { 330.0f,    0.0f,   0.0f }, /* E: 前面上端・右 */
    {   0.0f,    0.0f,   0.0f }, /* F: 前面上端・左 */
    {   0.0f,   33.0f, 512.0f }, /* G: 上面奥辺・左 */
    { 330.0f,   33.0f, 512.0f }, /* H: 上面奥辺・右 */
    { -79.0f,  -68.0f, 574.0f }, /* I */
    { -21.9f, -112.5f, 685.0f }  /* J */
};

/* F, G, I, J, B を通る側面図形を描画 */
/* scale : オブジェクトの縮尺 */
static void drawControlside(float scale)
{
    const Vec3* v = CONTROL_CENTER_VERTICES;

    glPushMatrix();
    glScalef(scale, scale, scale);

    /* 5頂点(F -> G -> I -> J -> B)を三角形分割して描画 */
    glBegin(GL_TRIANGLES);

    /* F -> G -> I */
    drawTriangleV(&v[V_F], &v[V_G], &v[V_I]);

    /* F -> I -> J */
    drawTriangleV(&v[V_F], &v[V_I], &v[V_J]);

    /* F -> J -> B */
    drawTriangleV(&v[V_F], &v[V_J], &v[V_B]);

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