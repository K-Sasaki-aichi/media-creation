#ifndef HANGAR_H
#define HANGAR_H

#include <GL/glut.h>
#include <math.h>
#include "../texture/texture.h"

void drawHangar(float groundWidth, float groundDepth, float height)
{
    float x = groundWidth / 2.0f;
    float z = groundDepth / 2.0f;

    /* 壁の傾斜による天井の縮小量 */
    float offset = height / sqrtf(3.0f);
    float topX = x - offset;

    /* 天井幅が確保できない場合 */
    if (topX <= 0.0f || height <= 0.0f)
        return;

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    /* =========================
       床
       ========================= */
    glBindTexture(GL_TEXTURE_2D, texid_ground);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, 0.0f, -z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-x, 0.0f, z);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x, 0.0f, z);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x, 0.0f, -z);

    glEnd();

    /* =========================
       天井
       ========================= */
    glBindTexture(GL_TEXTURE_2D, texid_ceiling);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, -1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-topX, height, -z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-topX, height, z);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(topX, height, z);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(topX, height, -z);

    glEnd();

    /* =========================
       左側の壁
       ========================= */
    glBindTexture(GL_TEXTURE_2D, texid_hwall);

    glBegin(GL_QUADS);

    glNormal3f(-0.8660254f, 0.5f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, 0.0f, -z);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-x, 0.0f, z);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-topX, height, z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-topX, height, -z);

    glEnd();

    /* =========================
       右側の壁
       ========================= */
    glBegin(GL_QUADS);

    glNormal3f(0.8660254f, 0.5f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, 0.0f, z);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x, 0.0f, -z);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(topX, height, -z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(topX, height, z);

    glEnd();

    /* =========================
       後ろ側の壁（+Z面）
       ========================= */
    glBindTexture(GL_TEXTURE_2D, texid_hwall);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x, 0.0f, z);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x, 0.0f, z);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(topX, height, z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-topX, height, z);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

#endif