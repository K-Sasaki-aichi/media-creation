#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include "kit.h"

#define PI 3.14159265358979323846

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[]   = { 0.8, 0.2, 0.2, 1.0 };

/* --- 視点と移動のための変数 --- */
float camX = 0.0f, camY = 0.0f, camZ = 5.0f; /* 初期位置 */
float rotX = 15.0f, rotY = 0.0f;            /* 初期角度 */

int isDragging = 0;
int lastMouseX = 0, lastMouseY = 0;




void drawCanopy(void)
{
    float width  = 1.4f;
    float height = 1.2f;
    // float depth  = 3.5f;

    float t = 0.1f;

    float xL = -width / 2.0f;
    float xR =  width / 2.0f;

    float zF = -0.8f;
    float zB =  4.0f;

    // 三角形の頂点
    float baseY = 0.0f;
    float topY  = height;

    // glColor3f(0.65f, 0.68f, 0.72f);

    // 左側面：三角形の3辺
    drawBeam(xL, baseY, zF, xL, topY, 0, t);
    drawBeam(xL, topY, 0, xL, baseY, zB, t);
    drawBeam(xL, baseY, zB, xL, baseY, zF, t);

    // 右側面：三角形の3辺
    drawBeam(xR, baseY, zF, xR, topY, 0, t);
    drawBeam(xR, topY, 0, xR, baseY, zB, t);
    drawBeam(xR, baseY, zB, xR, baseY, zF, t);


    
    // 左右の三角形をつなぐ直方体
    // 前方下部
    drawBeam(xL, baseY, zF, xR, baseY, zF, t);

    // 後方下部
    drawBeam(xL, baseY, zB, xR, baseY, zB, t);

    // 上部の頂点同士
    drawBeam(xL, topY, 0, xR, topY, 0, t);
}



void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* 視点・カメラの設定 */
    glTranslatef(-camX, -camY, -camZ);  /* 位置移動 */
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);  /* 上下回転 */
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);  /* 左右回転 */

    /* 光源の位置設定 */
    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

    /* 円筒を描画（半径1.0, 長さ3.0, 分割数32） */
    glPushMatrix();
        glColor3f(0.65f, 0.68f, 0.72f);
        drawCanopy();
    glPopMatrix();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = 1;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = 0;
        }
    }
}

void motion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        rotY += dx * 0.5f;
        rotX += dy * 0.5f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.2f;

    switch (key) {
        case 'w': case 'W': camZ -= moveSpeed; break; /* 前進（筒の中に近づく/入る） */
        case 's': case 'S': camZ += moveSpeed; break; /* 後退 */
        case 'a': case 'A': camX -= moveSpeed; break; /* 左移動 */
        case 'd': case 'D': camX += moveSpeed; break; /* 右移動 */
        case 'r': case 'R': camY += moveSpeed; break; /* 上移動 */
        case 'f': case 'F': camY -= moveSpeed; break; /* 下移動 */
        case 27: exit(0); break;                      /* ESCで終了 */
    }
    glutPostRedisplay();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* 近視野面(zNear)を 0.1 に設定して接近時・進入時の面カットを防止 */
    gluPerspective(30.0, (double)w / (double)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    /* 中に入るためカリングを無効化（裏面も描画） */
    //glDisable(GL_CULL_FACE);

    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);

    // glEnable(GL_LIGHT1);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, green);

    /* 筒の内部に入った時に内壁を正しく照らすため両面ライティングを有効化 */
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    // 背景色を黒に設定（RGBA）
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    
    init();
    glutMainLoop();
    return 0;
}