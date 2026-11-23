#ifndef CANOPY_H
#define CANOPY_H

#include <GL/glut.h>
#include "../objects/cuboid.h"
#include "../vector.h"
#include "../gl_draw.h"

typedef enum
{
    CANOPY_FRONT_L = 0,
    CANOPY_FRONT_R,
    CANOPY_MID_TOP_L,
    CANOPY_MID_TOP_R,
    CANOPY_MID_BOT_L,
    CANOPY_MID_BOT_R,
    CANOPY_REAR_TOP_L,
    CANOPY_REAR_TOP_R,
    CANOPY_REAR_TIP_L,
    CANOPY_REAR_TIP_R,
    CANOPY_VERTEX_COUNT
} CanopyVertexId;

static const Vec3 CANOPY_BASE_VERTICES[CANOPY_VERTEX_COUNT] = {
    {    0.0f,    0.0f,    0.0f }, /* CANOPY_FRONT_L: 前方頂点・左（原点） */
    {  330.0f,    0.0f,    0.0f }, /* CANOPY_FRONT_R: 前方頂点・右 */
    {  -70.0f,  205.0f, 1060.0f }, /* CANOPY_MID_TOP_L: 中央断面・上端・左 */
    {  400.0f,  205.0f, 1060.0f }, /* CANOPY_MID_TOP_R: 中央断面・上端・右 */
    { -147.5f, -125.0f, 1060.0f }, /* CANOPY_MID_BOT_L: 中央断面・下端・左 */
    {  477.5f, -125.0f, 1060.0f }, /* CANOPY_MID_BOT_R: 中央断面・下端・右 */
    {  -45.0f,  205.0f, 1263.0f }, /* CANOPY_REAR_TOP_L: 後方天板終端・左 */
    {  375.0f,  205.0f, 1263.0f }, /* CANOPY_REAR_TOP_R: 後方天板終端・右 */
    {  -45.0f,  115.0f, 1370.0f }, /* CANOPY_REAR_TIP_L: 後方尖端・左 */
    {  375.0f,  115.0f, 1370.0f }  /* CANOPY_REAR_TIP_R: 後方尖端・右 */
};

static void drawBeamV(const Vec3* start, const Vec3* end, float thickness)
{
    drawBeam(
        start->x, start->y, start->z,
        end->x, end->y, end->z,
        thickness
    );
}

static void drawCanopyGlass(const Vec3 v[])
{
    beginGlass();

    // 左前面のガラス
    glBegin(GL_TRIANGLES);
    glVertex3v(&v[CANOPY_FRONT_L]);
    glVertex3v(&v[CANOPY_MID_TOP_L]);
    glVertex3v(&v[CANOPY_MID_BOT_L]);
    glEnd();

    // 右前面のガラス
    glBegin(GL_TRIANGLES);
    glVertex3v(&v[CANOPY_FRONT_R]);
    glVertex3v(&v[CANOPY_MID_BOT_R]);
    glVertex3v(&v[CANOPY_MID_TOP_R]);
    glEnd();

    // 左側面のガラス
    glBegin(GL_QUADS);
    glVertex3v(&v[CANOPY_MID_TOP_L]);
    glVertex3v(&v[CANOPY_REAR_TOP_L]);
    glVertex3v(&v[CANOPY_REAR_TIP_L]);
    glVertex3v(&v[CANOPY_MID_BOT_L]);
    glEnd();

    // 右側面のガラス
    glBegin(GL_QUADS);
    glVertex3v(&v[CANOPY_MID_TOP_R]);
    glVertex3v(&v[CANOPY_MID_BOT_R]);
    glVertex3v(&v[CANOPY_REAR_TIP_R]);
    glVertex3v(&v[CANOPY_REAR_TOP_R]);
    glEnd();

    endGlass();
}

static void drawCanopy(float s)
{
    Vec3 v[CANOPY_VERTEX_COUNT];
    int i;
    const float t = 0.09f; /* 骨組みの太さ */

    /* スケール s を適用した頂点配列を作成 */
    for (i = 0; i < CANOPY_VERTEX_COUNT; i++)
    {
        v[i].x = CANOPY_BASE_VERTICES[i].x * s;
        v[i].y = CANOPY_BASE_VERTICES[i].y * s;
        v[i].z = CANOPY_BASE_VERTICES[i].z * s;
    }

    //drawCanopyGlass(v);

    /* ===================================== */
    /* 2. 前方部分                           */
    /* ===================================== */

    /* 前方頂点から中央上端へ */
    drawBeamV(&v[CANOPY_FRONT_L], &v[CANOPY_MID_TOP_L], t);
    drawBeamV(&v[CANOPY_FRONT_R], &v[CANOPY_MID_TOP_R], t);

    /* 前方頂点から中央下端へ */
    drawBeamV(&v[CANOPY_FRONT_L], &v[CANOPY_MID_BOT_L], t);
    drawBeamV(&v[CANOPY_FRONT_R], &v[CANOPY_MID_BOT_R], t);

    /* 前方の横骨 */
    drawBeamV(&v[CANOPY_FRONT_L], &v[CANOPY_FRONT_R], t);

    /* ===================================== */
    /* 3. 中央断面                           */
    /* ===================================== */

    /* 上側の横骨 */
    drawBeamV(&v[CANOPY_MID_TOP_L], &v[CANOPY_MID_TOP_R], t);

    /* 下側の横骨 */
    drawBeamV(&v[CANOPY_MID_BOT_L], &v[CANOPY_MID_BOT_R], t);

    /* 左右の側面骨 */
    drawBeamV(&v[CANOPY_MID_TOP_L], &v[CANOPY_MID_BOT_L], t);
    drawBeamV(&v[CANOPY_MID_TOP_R], &v[CANOPY_MID_BOT_R], t);

    /* ===================================== */
    /* 4. 中央から後方の天板終端             */
    /* ===================================== */

    drawBeamV(&v[CANOPY_MID_TOP_L], &v[CANOPY_REAR_TOP_L], t);
    drawBeamV(&v[CANOPY_MID_TOP_R], &v[CANOPY_REAR_TOP_R], t);

    /* ===================================== */
    /* 5. 中央下端から後方頂点               */
    /* ===================================== */

    drawBeamV(&v[CANOPY_MID_BOT_L], &v[CANOPY_REAR_TIP_L], t);
    drawBeamV(&v[CANOPY_MID_BOT_R], &v[CANOPY_REAR_TIP_R], t);

    /* ===================================== */
    /* 6. 後方の上端から尖った頂点           */
    /* ===================================== */

    /* 左側 */
    drawBeamV(&v[CANOPY_REAR_TOP_L], &v[CANOPY_REAR_TIP_L], t);

    /* 右側 */
    drawBeamV(&v[CANOPY_REAR_TOP_R], &v[CANOPY_REAR_TIP_R], t);

    /* ===================================== */
    /* 7. 後方の横骨                         */
    /* ===================================== */

    /* 天板終端 */
    drawBeamV(&v[CANOPY_REAR_TOP_L], &v[CANOPY_REAR_TOP_R], t);

    /* 後方頂点 */
    drawBeamV(&v[CANOPY_REAR_TIP_L], &v[CANOPY_REAR_TIP_R], t);

    /* ===================================== */
    /* 8. 天板                               */
    /* ===================================== */

    glBegin(GL_QUADS);
    glVertex3v(&v[CANOPY_MID_TOP_L]);
    glVertex3v(&v[CANOPY_REAR_TOP_L]);
    glVertex3v(&v[CANOPY_REAR_TOP_R]);
    glVertex3v(&v[CANOPY_MID_TOP_R]);
    glEnd();

    /* ===================================== */
    /* 9. 後方下部の天板                     */
    /* ===================================== */

    glBegin(GL_QUADS);
    glVertex3v(&v[CANOPY_MID_BOT_L]);
    glVertex3v(&v[CANOPY_MID_BOT_R]);
    glVertex3v(&v[CANOPY_REAR_TIP_R]);
    glVertex3v(&v[CANOPY_REAR_TIP_L]);
    
    glEnd();
}

#endif