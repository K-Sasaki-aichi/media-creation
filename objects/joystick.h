
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <GL/glut.h>
#include <math.h>

/* 操縦桿の円柱 */
static void drawStickCylinder(float radius, float height)
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluCylinder(quad, radius, radius, height, 16, 2);

    gluDeleteQuadric(quad);
}

/* 操縦桿 */
static void drawControlStick(void)
{
    glPushMatrix();

    /* 台座 */
    glColor3f(0.18f, 0.19f, 0.19f);
    glPushMatrix();
    glScalef(65.0f, 15.0f, 65.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    /* 支柱：円柱はZ軸方向なのでX軸回転 */
    glColor3f(0.38f, 0.39f, 0.38f);
    glPushMatrix();
    glTranslatef(0.0f, 7.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawStickCylinder(9.0f, 65.0f);
    glPopMatrix();

    /* 握り部分 */
    glColor3f(0.12f, 0.13f, 0.13f);
    glPushMatrix();
    glTranslatef(0.0f, 75.0f, 0.0f);
    glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
    glScalef(30.0f, 35.0f, 24.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    /* 赤いボタン */
    glColor3f(0.8f, 0.05f, 0.03f);
    glPushMatrix();
    glTranslatef(0.0f, 92.0f, -8.0f);
    glutSolidSphere(5.0f, 12, 12);
    glPopMatrix();

    glPopMatrix();
}

#endif
