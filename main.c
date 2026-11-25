#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#include "models/canopy.h"
#include "models/cockpit.h"
#include "texture/texture.h"
#include "models/planets.h"
#include "objects/hyperspace.h"
#include "objects/stars.h"
#include "objects/sphere.h"
#include "models/hangar.h"
#include "models/asteroids.h"

#define INITIAL_CAM_X 0.825f
#define INITIAL_CAM_Y 0.28f
#define INITIAL_CAM_Z 5.0f
#define HANGAR_WAIT_FRAMES 50
#define PI 3.14159265358979323846

/* 光源・マテリアル設定 */
GLfloat light0pos[]            = { 0.0f, 3.0f, 5.0f, 1.0f };
GLfloat light1pos[]            = { 5.0f, 3.0f, 0.0f, 1.0f };
GLfloat cockpitLightPos[]      = { 0.8f, 0.2f, 2.5f, 1.0f };
GLfloat cockpitLightDiffuse[]  = { 0.8f, 0.8f, 0.7f, 1.0f };
GLfloat cockpitLightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat green[]                = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat red[]                  = { 0.8f, 0.2f, 0.2f, 1.0f };

/* カメラ・視線 */
float camX = INITIAL_CAM_X;
float camY = INITIAL_CAM_Y;
float camZ = INITIAL_CAM_Z;
float rotX = 12.0f;
float rotY = 0.0f;

/* 機体ステータス */
float xwingRy = 0.0f, xwingRx = 0.0f;
float shipMoveX = 0.0f, shipMoveY = 0.0f, shipMoveZ = 0.0f;
float isBoost = 0.0f;

/* ハンガー・発進アニメーション状態 */
int isHanger = 1;
int isLaunching = 0;
int hangarFrames = 0;
float liftOffsetY = 0.0f;
float advanceOffsetZ = 0.0f;

/* その他状態変数 */
Vec3 center;
int isDragging = 0;
int lastMouseX = 0, lastMouseY = 0;
int frameCount = 0;

void initXwing(void)
{
    shipMoveX = 0.0f;
    shipMoveY = 0.0f;
    shipMoveZ = 0.0f;
    xwingRy = 0.0f;
    xwingRx = 0.0f;
}

void startLaunch(void)
{
    if (!isHanger || isLaunching || isHyperspaceActive()) return;

    initXwing();
    liftOffsetY = 0.0f;
    advanceOffsetZ = 0.0f;
    isLaunching = 1;
}

void updateLaunch(void)
{
    const float targetLiftY = 2.50f;     /* 浮き上がる高さ */
    const float liftSpeed = 0.01f;       /* 浮上速度 */
    const float advanceSpeed = 0.2f;     /* まっすぐ進む速度 */
    const float exitDistance = 80.0f;    /* ハンガーをある程度抜ける距離 */

    if (!isLaunching) return;

    if (liftOffsetY < targetLiftY) {
        liftOffsetY += liftSpeed;
        if (liftOffsetY > targetLiftY) {
            liftOffsetY = targetLiftY;
        }
        return;
    }

    advanceOffsetZ += advanceSpeed;
    if (advanceOffsetZ >= exitDistance) {
        isLaunching = 0;
        isHanger = 0;
        hangarFrames = 0;
        liftOffsetY = 0.0f;
        advanceOffsetZ = 0.0f;
        initXwing();
        startHyperspace();
    }
}

GLuint swapTex(GLuint tex)
{
    return (tex == texid_control_center0) ? texid_control_center1 : texid_control_center0;
}

Vec3 getDir(void)
{
    float radY = xwingRy * PI / 180.0f;
    float radX = xwingRx * PI / 180.0f;

    return (Vec3){
        sinf(radY),
        sinf(radX) * cosf(radY),
        -cosf(radX) * cosf(radY)
    };
}

void drawHangarScene(void)
{
    glPushMatrix();
    /* 機体が浮上・前進した分だけハンガーを相対移動 */
    glTranslatef(0.825f, -2.2f - liftOffsetY, -10.5f + advanceOffsetZ);
    drawHangar(56.0f, 90.0f, 11.0f);
    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* カメラ・視線設定 */
    float yaw = rotY * PI / 180.0f;
    float pitch = rotX * PI / 180.0f;
    float dirX = sinf(yaw) * cosf(pitch);
    float dirY = -sinf(pitch);
    float dirZ = -cosf(yaw) * cosf(pitch);

    gluLookAt(camX, camY, camZ, camX + dirX, camY + dirY, camZ + dirZ, 0.0f, 1.0f, 0.0f);

    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT1, GL_POSITION, cockpitLightPos);

    float s = 0.005f;
    float moveSpeed = isBoost ? 0.2f : 0.02f;

    if (isHanger) {
        hangarFrames++;
        glPushMatrix();
        glTranslatef(0, 0, -60);
        drawStars();
        glPopMatrix();
        
        if (hangarFrames >= HANGAR_WAIT_FRAMES) {
            if (!isLaunching) {
                startLaunch();
            }
            updateLaunch();
        }
        drawHangarScene();
    }

    /* 星空・惑星の更新と描画 */
    if (hasDrawStars() && !isHanger) {
        Vec3 dir = getDir();
        shipMoveX += dir.x * moveSpeed;
        shipMoveY += dir.y * moveSpeed;
        shipMoveZ += dir.z * moveSpeed;

        /* 一定距離移動でハイパースペースへ移行 */
        float distance2 = shipMoveX * shipMoveX + shipMoveY * shipMoveY + shipMoveZ * shipMoveZ;
        if (distance2 > 100.0f * 100.0f && !isHyperspaceActive()) {
            initXwing();
            startHyperspace();
        }

        center = (Vec3){ INITIAL_CAM_X, INITIAL_CAM_Y, INITIAL_CAM_Z };

        /* カメラ基準の相対座標に変換して描画 */
        glPushMatrix();
        glTranslatef(camX, camY, camZ);
        glRotatef(xwingRy, 0.0f, 1.0f, 0.0f);
        glRotatef(-xwingRx, 1.0f, 0.0f, 0.0f);
        drawStars();

        glPushMatrix();
        glTranslatef(-camX - shipMoveX, -camY - shipMoveY, -camZ - shipMoveZ);
        drawAsteroids(center, s);
        drawPlanets(center, s);
        glPopMatrix();

        glPopMatrix();
    }

    /* コックピット・ハイパースペース描画 */
    drawCockpit(s);
    drawHyperspace();

    if ((frameCount % 30) == 0) {
        texid_control_center = swapTex(texid_control_center);
    }
    frameCount++;

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
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

void motion(int x, int y)
{
    if (!isDragging) return;

    rotY += (x - lastMouseX) * 0.5f;
    rotX += (y - lastMouseY) * 0.5f;

    if (rotX > 85.0f)  rotX = 85.0f;
    if (rotX < -85.0f) rotX = -85.0f;

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    /* 発進アニメーション中はキー入力を受け付けない */
    if (isLaunching) return;

    if (key == 13 || key == '\n') {
        if (isHanger) return;
        initXwing();
        startHyperspace();
        return;
    }
    if (isHyperspaceActive()) return;

    float moveSpeed = 0.05f;
    float rotStep = 1.0f;

    switch (key) {
        case 'd': case 'D': xwingRy += rotStep;   break;
        case 'a': case 'A': xwingRy -= rotStep;   break;
        case 'w': case 'W': xwingRx += rotStep;   break;
        case 's': case 'S': xwingRx -= rotStep;   break;
        case 'e': case 'E':
        case ' ':           shipMoveY += moveSpeed; break;
        case 'q': case 'Q': shipMoveY -= moveSpeed; break;
        case 'j': case 'J': isBoost = 1.0f;       break;
        case 27:            exit(0);              break;
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;
    if (key == 'j' || key == 'J') {
        isBoost = 0.0f;
    }
}

void resize(int w, int h)
{
    if (h <= 0 || w <= 0) return;

    const double aspect = 2.35;
    double windowAspect = (double)w / (double)h;
    int viewportW, viewportH;

    if (windowAspect > aspect) {
        viewportH = h;
        viewportW = (int)(h * aspect);
    } else {
        viewportW = w;
        viewportH = (int)(w / aspect);
    }

    glViewport((w - viewportW) / 2, (h - viewportH) / 2, viewportW, viewportH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    center = (Vec3){ INITIAL_CAM_X, INITIAL_CAM_Y, INITIAL_CAM_Z };
    glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1200, 511);
    glutCreateWindow(argv[0]);

    initTextures();
    initHyperspace();
    initStars();
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}