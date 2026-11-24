#ifndef CANOPY_H
#define CANOPY_H

#include <GL/glut.h>
#include "../objects/cuboid.h"
#include "../vector.h"
#include "../gl_draw.h"
#include "../texture/texture.h"
#include "../objects/glass.h"

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

static void drawBeamV(const Vec3* start, const Vec3* end, float thickness, GLuint textureID)
{
    drawBeam(
        start->x, start->y, start->z,
        end->x, end->y, end->z,
        thickness,
        textureID
    );
}

static void drawCanopyGlass(const Vec3 v[])
{
    const float edge_alpha = 0.22f;
    const float center_alpha = 0.05f;

    GLboolean was_texture_2d = glIsEnabled(GL_TEXTURE_2D);
    GLboolean was_blend = glIsEnabled(GL_BLEND);
    GLboolean was_color_material = glIsEnabled(GL_COLOR_MATERIAL);

    if (was_texture_2d){
        glDisable(GL_TEXTURE_2D);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat glass_specular[] = { 1.0f, 1.0f, 1.0f, 0.9f };
    GLfloat glass_emission[] = { 0.02f, 0.04f, 0.06f, 0.0f };
    GLfloat glass_shininess  = 110.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glass_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glass_emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, glass_shininess);

/* 前面のガラス（後ろ下・内側向き） */
    drawGlassQuad(
        &v[CANOPY_FRONT_L],
        &v[CANOPY_FRONT_R],
        &v[CANOPY_MID_TOP_R],
        &v[CANOPY_MID_TOP_L],
        edge_alpha*1.6,
        center_alpha
    );

    /* 左前面のガラス（右後ろ・内側向き） */
    drawGlassQuad(
        &v[CANOPY_FRONT_L],
        &v[CANOPY_MID_TOP_L],
        &v[CANOPY_MID_BOT_L],
        &v[CANOPY_MID_BOT_L],
        edge_alpha,
        center_alpha
    );

    /* 右前面のガラス（左後ろ・内側向き） */
    drawGlassQuad(
        &v[CANOPY_FRONT_R],
        &v[CANOPY_MID_BOT_R],
        &v[CANOPY_MID_TOP_R],
        &v[CANOPY_MID_TOP_R],
        edge_alpha,
        center_alpha
    );

    /* 左側面のガラス（右・内側向き） */
    drawGlassQuad(
        &v[CANOPY_MID_TOP_L],
        &v[CANOPY_REAR_TOP_L],
        &v[CANOPY_REAR_TIP_L],
        &v[CANOPY_MID_BOT_L],
        edge_alpha,
        center_alpha
    );

    /* 右側面のガラス（左・内側向き） */
    drawGlassQuad(
        &v[CANOPY_MID_TOP_R],
        &v[CANOPY_MID_BOT_R],
        &v[CANOPY_REAR_TIP_R],
        &v[CANOPY_REAR_TOP_R],
        edge_alpha,
        center_alpha
    );

    /* 背面のガラス（前・内側向きに反転） */
    drawGlassQuad(
        &v[CANOPY_REAR_TOP_R],
        &v[CANOPY_REAR_TIP_R],
        &v[CANOPY_REAR_TIP_L],
        &v[CANOPY_REAR_TOP_L],
        edge_alpha*4,
        center_alpha
    );


    GLfloat reset_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat reset_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, reset_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, reset_emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);


    if (!was_color_material) {
        glDisable(GL_COLOR_MATERIAL);
    }
    glDepthMask(GL_TRUE);
    if (!was_blend) {
        glDisable(GL_BLEND);
    }
    if (was_texture_2d) {
        glEnable(GL_TEXTURE_2D);
    }
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
    drawBeamV(&v[CANOPY_FRONT_L], &v[CANOPY_MID_TOP_L], t, texid_canopy);
    drawBeamV(&v[CANOPY_FRONT_R], &v[CANOPY_MID_TOP_R], t, texid_canopy);

    /* 前方頂点から中央下端へ */
    drawBeamV(&v[CANOPY_FRONT_L], &v[CANOPY_MID_BOT_L], t, texid_canopy);
    drawBeamV(&v[CANOPY_FRONT_R], &v[CANOPY_MID_BOT_R], t, texid_canopy);

    /* 前方の横骨 */
    drawBeamV(&v[CANOPY_FRONT_L], &v[CANOPY_FRONT_R], t, texid_canopy);

    /* ===================================== */
    /* 3. 中央断面                           */
    /* ===================================== */

    /* 上側の横骨 */
    drawBeamV(&v[CANOPY_MID_TOP_L], &v[CANOPY_MID_TOP_R], t, texid_canopy);

    /* 下側の横骨 */
    drawBeamV(&v[CANOPY_MID_BOT_L], &v[CANOPY_MID_BOT_R], t, texid_canopy);

    /* 左右の側面骨 */
    drawBeamV(&v[CANOPY_MID_TOP_L], &v[CANOPY_MID_BOT_L], t, texid_canopy);
    drawBeamV(&v[CANOPY_MID_TOP_R], &v[CANOPY_MID_BOT_R], t, texid_canopy);

    /* ===================================== */
    /* 4. 中央から後方の天板終端             */
    /* ===================================== */

    drawBeamV(&v[CANOPY_MID_TOP_L], &v[CANOPY_REAR_TOP_L], t, texid_canopy);
    drawBeamV(&v[CANOPY_MID_TOP_R], &v[CANOPY_REAR_TOP_R], t, texid_canopy);

    /* ===================================== */
    /* 5. 中央下端から後方頂点               */
    /* ===================================== */

    drawBeamV(&v[CANOPY_MID_BOT_L], &v[CANOPY_REAR_TIP_L], t, texid_canopy);
    drawBeamV(&v[CANOPY_MID_BOT_R], &v[CANOPY_REAR_TIP_R], t, texid_canopy);

    /* ===================================== */
    /* 6. 後方の上端から尖った頂点           */
    /* ===================================== */

    /* 左側 */
    drawBeamV(&v[CANOPY_REAR_TOP_L], &v[CANOPY_REAR_TIP_L], t, texid_canopy);

    /* 右側 */
    drawBeamV(&v[CANOPY_REAR_TOP_R], &v[CANOPY_REAR_TIP_R], t, texid_canopy);

    /* ===================================== */
    /* 7. 後方の横骨                         */
    /* ===================================== */

    /* 天板終端 */
    drawBeamV(&v[CANOPY_REAR_TOP_L], &v[CANOPY_REAR_TOP_R], t, texid_canopy);

    /* 後方頂点 */
    drawBeamV(&v[CANOPY_REAR_TIP_L], &v[CANOPY_REAR_TIP_R], t, texid_canopy);

    /* ===================================== */
    /* 8. 天板                               */
    /* ===================================== */

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texid_canopy);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3v(&v[CANOPY_MID_TOP_L]);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3v(&v[CANOPY_REAR_TOP_L]);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3v(&v[CANOPY_REAR_TOP_R]);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3v(&v[CANOPY_MID_TOP_R]);

    glEnd();

    /* ===================================== */
    /* 9. 後方下部の天板                     */
    /* ===================================== */

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3v(&v[CANOPY_MID_BOT_L]);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3v(&v[CANOPY_MID_BOT_R]);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3v(&v[CANOPY_REAR_TIP_R]);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3v(&v[CANOPY_REAR_TIP_L]);

    glEnd();

    drawCanopyGlass(v);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

#endif