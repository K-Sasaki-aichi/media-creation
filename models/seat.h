
#ifndef SEAT_H
#define SEAT_H

#include <GL/glut.h>

/* 直方体 */
static void drawSeatBox(float w, float h, float d)
{
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

/* 操縦席 */
static void drawSeat(void)
{
    glPushMatrix();

    /* 座面 */
    glColor3f(0.13f, 0.14f, 0.14f);
    glPushMatrix();
    glTranslatef(0.0f, -240.0f, 0.0f);
    drawSeatBox(170.0f, 30.0f, 170.0f);
    glPopMatrix();

    /* 背もたれ */
    glColor3f(0.19f, 0.20f, 0.20f);
    glPushMatrix();
    glTranslatef(0.0f, -145.0f, 65.0f);
    glRotatef(-8.0f, 1.0f, 0.0f, 0.0f);
    drawSeatBox(170.0f, 350.0f, 35.0f);
    glPopMatrix();

    /* 左肘掛け */
    glColor3f(0.24f, 0.25f, 0.24f);
    glPushMatrix();
    glTranslatef(-100.0f, -205.0f, -5.0f);
    drawSeatBox(30.0f, 35.0f, 140.0f);
    glPopMatrix();

    /* 右肘掛け */
    glPushMatrix();
    glTranslatef(100.0f, -205.0f, -5.0f);
    drawSeatBox(30.0f, 35.0f, 140.0f);
    glPopMatrix();

    /* 座席の脚 */
    glColor3f(0.32f, 0.33f, 0.33f);
    glPushMatrix();
    glTranslatef(0.0f, -270.0f, 0.0f);
    drawSeatBox(65.0f, 40.0f, 65.0f);
    glPopMatrix();

    glPopMatrix();
}

#endif
