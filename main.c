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
float rotX = 0.0f, rotY = 0.0f;            /* 初期角度 */

int isDragging = 0;
int lastMouseX = 0, lastMouseY = 0;




// void drawCanopy(void)
// {
//     float width  = 1.4f;
//     float height = 1.2f;
//     // float depth  = 3.5f;

//     float t = 0.1f;

//     float xL = -width / 2.0f;
//     float xR =  width / 2.0f;

//     float zF = -0.8f;
//     float zB =  4.0f;

//     // 三角形の頂点
//     float baseY = 0.0f;
//     float topY  = height;

//     // glColor3f(0.65f, 0.68f, 0.72f);

//     // 左側面：三角形の3辺
//     drawBeam(xL, baseY, zF, xL, topY, 0, t);
//     drawBeam(xL, topY, 0, xL, baseY, zB, t);
//     drawBeam(xL, baseY, zB, xL, baseY, zF, t);

//     // 右側面：三角形の3辺
//     drawBeam(xR, baseY, zF, xR, topY, 0, t);
//     drawBeam(xR, topY, 0, xR, baseY, zB, t);
//     drawBeam(xR, baseY, zB, xR, baseY, zF, t);


    
//     // 左右の三角形をつなぐ直方体
//     // 前方下部
//     drawBeam(xL, baseY, zF, xR, baseY, zF, t);

//     // 後方下部
//     drawBeam(xL, baseY, zB, xR, baseY, zB, t);

//     // 上部の頂点同士
//     drawBeam(xL, topY, 0, xR, topY, 0, t);
// }




void drawCanopy(float s)
{
    float t = 0.09f;

    // ================================
    // 寸法
    // ================================

    // 前方
    float frontZ = -16.0f * s;
    float frontY = 2.5f * s;
    float frontW = 7.0f * s / 2.0f;

    // 中央
    float centerZ = 0.0f;
    float centerTopY = 5.0f * s;

    float centerBottomW = 12.0f * s / 2.0f;
    float centerTopW = 9.0f * s / 2.0f;

    // 後方
    float rearZ = 6.0f * s;
    float rearY = 5.0f * s;
    float rearW = 8.0f * s / 2.0f;

    glColor3f(0.65f, 0.68f, 0.72f);

    // ================================
    // 1. 前方の尖った部分
    // ================================

    // 左側面：前方の頂点から中央上下へ
    drawBeam(
        -frontW, frontY, frontZ,
        -centerTopW, centerTopY, centerZ, t
    );

    drawBeam(
        -frontW, frontY, frontZ,
        -centerBottomW, 0.0f, centerZ, t
    );

    // 右側面
    drawBeam(
         frontW, frontY, frontZ,
         centerTopW, centerTopY, centerZ, t
    );

    drawBeam(
         frontW, frontY, frontZ,
         centerBottomW, 0.0f, centerZ, t
    );

    // 前方の横幅
    drawBeam(
        -frontW, frontY, frontZ,
         frontW, frontY, frontZ, t
    );

    // ================================
    // 2. 中央の断面
    // ================================

    // 上部の横骨組み
    drawBeam(
        -centerTopW, centerTopY, centerZ,
         centerTopW, centerTopY, centerZ, t
    );

    // 下部の横骨組み
    drawBeam(
        -centerBottomW, 0.0f, centerZ,
         centerBottomW, 0.0f, centerZ, t
    );

    // 左右の側面をつなぐ縦骨組み
    drawBeam(
        -centerBottomW, 0.0f, centerZ,
        -centerTopW, centerTopY, centerZ, t
    );

    drawBeam(
         centerBottomW, 0.0f, centerZ,
         centerTopW, centerTopY, centerZ, t
    );

    // ================================
    // 3. 後方への接続
    // ================================

    // 左上
    drawBeam(
        -centerTopW, centerTopY, centerZ,
        -rearW, rearY, rearZ, t
    );

    // 右上
    drawBeam(
         centerTopW, centerTopY, centerZ,
         rearW, rearY, rearZ, t
    );

    // 左下
    drawBeam(
        -centerBottomW, 0.0f, centerZ,
        -rearW, 0.0f, rearZ, t
    );

    // 右下
    drawBeam(
         centerBottomW, 0.0f, centerZ,
         rearW, 0.0f, rearZ, t
    );

    // 後方の断面
    drawBeam(
        -rearW, rearY, rearZ,
         rearW, rearY, rearZ, t
    );

    drawBeam(
        -rearW, 0.0f, rearZ,
         rearW, 0.0f, rearZ, t
    );

    drawBeam(
        -rearW, 0.0f, rearZ,
        -rearW, rearY, rearZ, t
    );

    drawBeam(
         rearW, 0.0f, rearZ,
         rearW, rearY, rearZ, t
    );

    // ================================
    // 4. 後方の天板
    // ================================

    glColor3f(0.65f, 0.68f, 0.72f);

    glBegin(GL_QUADS);

    // 中央から後方にかけての上面
    glVertex3f(-centerTopW, centerTopY, centerZ);
    glVertex3f( centerTopW, centerTopY, centerZ);
    glVertex3f( rearW, rearY, rearZ);
    glVertex3f(-rearW, rearY, rearZ);

    glEnd();
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
        drawCanopy(0.5);
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
        rotX += dy * 0;

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

        /* 上移動（R に加えて E と スペースキー を追加） */
        case 'r': case 'R':
        case 'e': case 'E':
        case ' ':
            camY += moveSpeed;
            break;

        /* 下移動（F に加えて Q と C を追加） */
        case 'f': case 'F':
        case 'q': case 'Q':
        case 'c': case 'C':
            camY -= moveSpeed;
            break;

        case 27: exit(0); break; /* ESCで終了 */
    }
    glutPostRedisplay();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* 近視野面(zNear)を 0.1 に設定して接近時・進入時の面カットを防止 */
    gluPerspective(45.0, (double)w / (double)h, 0.1, 100.0);

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
