
#include <GL/glut.h>
#include <math.h>

/* --- 視点と移動のための変数 --- */
float camX = 0.0f, camY = 0.0f, camZ = 5.0f; /* 初期位置 */
float rotX = 15.0f, rotY = 0.0f;            /* 初期角度 */

int isDragging = 0;
int lastMouseX = 0, lastMouseY = 0;

// 立体的な四角形パネル
void quad(
    float v[4][3],
    float r, float g, float b
) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
        glVertex3fv(v[i]);
    glEnd();
}

// 金属フレームの直方体
void drawBeam(float x1, float y1, float z1,
              float x2, float y2, float z2,
              float width)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;

    float len = sqrtf(dx*dx + dy*dy + dz*dz);

    glPushMatrix();
    glTranslatef((x1+x2)/2, (y1+y2)/2, (z1+z2)/2);

    float angle = acosf(dz / len) * 180.0f / 3.14159265f;
    glRotatef(angle, -dy, dx, 0);

    glColor3f(0.72f, 0.70f, 0.64f);
    glScalef(width, width, len);
    glutSolidCube(1.0);

    glPopMatrix();
}

// キャノピー
void drawCanopy(void)
{
    // 下側の外周
    float bottom[4][3] = {
        {-2.0f, 0.0f, -3.5f},
        { 2.0f, 0.0f, -3.5f},
        { 2.0f, 0.0f,  3.5f},
        {-2.0f, 0.0f,  3.5f}
    };

    // 上側の外周
    float top[4][3] = {
        {-1.5f, 1.5f, -2.5f},
        { 1.5f, 1.5f, -2.5f},
        { 1.5f, 1.5f,  2.5f},
        {-1.5f, 1.5f,  2.5f}
    };

    // 左右のガラス面
    float leftGlass[4][3] = {
        {-2.0f, 0.0f, -3.5f},
        {-1.5f, 1.5f, -2.5f},
        {-1.5f, 1.5f,  2.5f},
        {-2.0f, 0.0f,  3.5f}
    };

    float rightGlass[4][3] = {
        {2.0f, 0.0f, -3.5f},
        {2.0f, 0.0f,  3.5f},
        {1.5f, 1.5f,  2.5f},
        {1.5f, 1.5f, -2.5f}
    };

    // 上面ガラス
    float roof[4][3] = {
        {-1.5f, 1.5f, -2.5f},
        { 1.5f, 1.5f, -2.5f},
        { 1.5f, 1.5f,  2.5f},
        {-1.5f, 1.5f,  2.5f}
    };

    // ガラス
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.04f, 0.20f, 0.29f, 0.85f);

    glBegin(GL_QUADS);
    for(int i=0;i<4;i++)
        glVertex3fv(leftGlass[i]);
    glEnd();

    glBegin(GL_QUADS);
    for(int i=0;i<4;i++)
        glVertex3fv(rightGlass[i]);
    glEnd();

    glBegin(GL_QUADS);
    for(int i=0;i<4;i++)
        glVertex3fv(roof[i]);
    glEnd();

    glDisable(GL_BLEND);

    // 外周フレーム
    for(int i=0;i<4;i++) {
        int j=(i+1)%4;

        drawBeam(
            bottom[i][0], bottom[i][1], bottom[i][2],
            bottom[j][0], bottom[j][1], bottom[j][2],
            0.16f
        );

        drawBeam(
            top[i][0], top[i][1], top[i][2],
            top[j][0], top[j][1], top[j][2],
            0.13f
        );

        drawBeam(
            bottom[i][0], bottom[i][1], bottom[i][2],
            top[i][0], top[i][1], top[i][2],
            0.16f
        );
    }

    // 中央の補強フレーム
    drawBeam(0.0f, 1.5f, -2.5f,
             0.0f, 1.5f,  2.5f, 0.12f);

    drawBeam(-1.5f, 1.5f, 0.0f,
              1.5f, 1.5f, 0.0f, 0.12f);
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


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // カメラの回転
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // カメラの移動
    glTranslatef(-camX, -camY, -camZ);

    drawCanopy();

    glutSwapBuffers();
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
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);

    /* 中に入るためカリングを無効化（裏面も描画） */
    //glDisable(GL_CULL_FACE);

    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);

    // glEnable(GL_LIGHT1);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, green);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    
    init();
    glutMainLoop();
    return 0;
}