#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "models/canopy.h"
#include "models/control.h"
#include "objects/boxframe.h"

#define PI 3.14159265358979323846

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[]   = { 0.8, 0.2, 0.2, 1.0 };

/* --- 視点と移動のための変数 --- */
// パイロットの頭の位置
float camX = 0.0f;
float camY = 0.0f;
float camZ = 0.0f;

// 視線の角度
float rotX = 0.0f;
float rotY = 0.0f;

int isDragging = 0;
int lastMouseX = 0, lastMouseY = 0;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* 光源の位置設定 */
    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1pos);


    /* 視点・カメラの設定 */
    glTranslatef(-camX, -camY, -camZ);  /* 位置移動 */
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);  /* 上下回転 */
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);  /* 左右回転 */

    float s = 0.005f;

    glColor3f(1, 1, 1);
    drawCanopy(s);

    // 操縦桿の中央部分
    glColor3f(0.729f, 0.737f, 0.741f);
    drawControlCenter(s);
    drawControlside(s);

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
        rotX += dy * 0.5;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.05f;

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
    const double aspect = 2.35;

    int viewportW, viewportH;
    int viewportX, viewportY;

    double windowAspect = (double)w / (double)h;

    if (windowAspect > aspect) {
        // ウィンドウが横に広すぎる場合
        viewportH = h;
        viewportW = (int)(h * aspect);
    }
    else {
        // ウィンドウが縦に長すぎる場合
        viewportW = w;
        viewportH = (int)(w / aspect);
    }

    viewportX = (w - viewportW) / 2;
    viewportY = (h - viewportH) / 2;

    glViewport(viewportX, viewportY, viewportW, viewportH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    /* ライティングと光源を有効化 */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /* glColor3f の色を面の拡散反射・環境光に反映させる */
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    /* 筒の内部に入った時に内壁を正しく照らすため両面ライティングを有効化 */
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    /* 必要であれば GL_LIGHT1 も有効化 */
    // glEnable(GL_LIGHT1);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, green);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1200, 511);
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
