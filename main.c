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

#define PI 3.14159265358979323846

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };

/* コックピット内部用の光源位置（s = 0.005f 基準の内部目安） */
GLfloat cockpitLightPos[] = { 0.8f, 0.2f, 2.5f, 1.0f };
GLfloat cockpitLightDiffuse[] = { 0.8f, 0.8f, 0.7f, 1.0f };
GLfloat cockpitLightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[]   = { 0.8, 0.2, 0.2, 1.0 };

/* --- 視点と移動のための変数 --- */
// パイロットの頭の位置（コックピット中央・やや後方）
float camX = 0.825f;
float camY = 0.35f;
float camZ = 5.2f;

Vec3 center;


// 視線の角度（0, 0 で -Z 方向＝前方。rotX を少しプラスにするとやや下向き）
float rotX = 12.0f;
float rotY = 0.0f;

float xwingRy = 0, xwingRx = 0;
float xwingX = 0, xwingY = 0, xwingZ = 0;
// 惑星の移動量（機体の進行方向と逆向き）
float planetMoveX = 0.0f;
float planetMoveY = 0.0f;
float planetMoveZ = 0.0f;

int isDragging = 0;
int lastMouseX = 0, lastMouseY = 0;

int frameCount = 0;

GLuint swapTex(GLuint tex){
    if(tex == texid_control_center0) return texid_control_center1;
    else return texid_control_center0;
}

Vec3 getDir(){
    // 機体の角度（度）をラジアンに変換
    float radY = xwingRy * PI / 180.0f;
    float radX = xwingRx * PI / 180.0f;

    // 機体が向いている方向
    float dirX = -sinf(radY) * cosf(radX);
    float dirY =  sinf(radX);
    float dirZ = -cosf(radY) * cosf(radX);

    return (Vec3){dirX, dirY, dirZ};
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 視線方向の計算
    float yaw   = rotY * PI / 180.0f;
    float pitch = rotX * PI / 180.0f;

    float dirX = sin(yaw) * cos(pitch);
    float dirY = -sin(pitch);
    float dirZ = -cos(yaw) * cos(pitch);

    gluLookAt(
        camX, camY, camZ,
        camX + dirX,
        camY + dirY,
        camZ + dirZ,
        0.0f, 1.0f, 0.0f
    );

    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT1, GL_POSITION, cockpitLightPos);

    float moveSpeed = 0.01f;

    float s = 0.005f;
if (hasDrawStars()) {

    // 機体の進行方向を取得
    Vec3 dir = getDir();

    // 進行方向と逆向きに惑星を移動
    planetMoveX -= dir.x * moveSpeed;
    planetMoveY -= dir.y * moveSpeed;
    planetMoveZ -= dir.z * moveSpeed;

    // カメラ位置を更新
    center = (Vec3){camX, camY, camZ};

    glPushMatrix();
    // カメラ位置を基準にする
    glTranslatef(camX, camY, camZ);

    // 累積した移動量を適用（回転させない）
    glTranslatef(planetMoveX, planetMoveY,planetMoveZ);
    // 機体の旋回に対応した宇宙空間の回転
    glRotatef(xwingRy, 0.0f, 1.0f, 0.0f);
    glRotatef(-xwingRx, 1.0f, 0.0f, 0.0f);

    glTranslatef(-camX, -camY, -camZ);
    drawPlanets(center, s);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(camX, camY, camZ);

    glRotatef(xwingRy, 0.0f, 1.0f, 0.0f);
    glRotatef(-xwingRx, 1.0f, 0.0f, 0.0f);

    glTranslatef(-camX, -camY, -camZ);
    drawStars();
    glPopMatrix();
}

    drawCockpit(s);
    drawHyperspace();
    

    if((frameCount % 30) == 0) texid_control_center = swapTex(texid_control_center);
    frameCount++;

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

void specialKey(int key, int x, int y) {
    (void)x;
    (void)y;
    float moveSpeed = 0.05f;

    if (isHyperspaceActive()) {
        return;
    }

    switch (key) {
        case GLUT_KEY_UP:    camZ -= moveSpeed; break; /* 前進 */
        case GLUT_KEY_DOWN:  camZ += moveSpeed; break; /* 後退 */
        case GLUT_KEY_LEFT:  camX -= moveSpeed; break; /* 左移動 */
        case GLUT_KEY_RIGHT: camX += moveSpeed; break; /* 右移動 */
        default: return;
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    float moveSpeed = 0.05f;
    float rotStep = 1.0f; // glRotatef は「度」指定なので数度単位にする

    /* ENTERキーでハイパースペース発動 */
    if (key == 13 || key == '\n') {
        startHyperspace();
        return;
    }

    /* ハイパースペース中は移動操作を受け付けない場合 */
    if (isHyperspaceActive()) {
        return;
    }

    switch (key) {
        case 'd': case 'D': xwingRy += rotStep; break;
        case 'a': case 'A': xwingRy -= rotStep; break;

        case 'w': case 'W': xwingRx += rotStep; break;
        case 's': case 'S': xwingRx -= rotStep; break;

        /* 上移動 */
        case 'e': case 'E':
        case ' ':
            camY += moveSpeed;
            break;

        /* 下移動 */
        case 'q': case 'Q':
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

    gluPerspective(45.0, aspect, 0.1, 200.0);

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

    center = (Vec3){camX, camY, camZ};
    /* 必要であれば GL_LIGHT1 も有効化 */
    // glEnable(GL_LIGHT1);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, green);
}

void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1200, 511);
    glutCreateWindow(argv[0]);
    initTextures();
    initHyperspace();
    
    //glutTimerFunc(16, updateHyperspace, 0);
    // 背景色を黒に設定（RGBA）
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    glutIdleFunc(idle);
    
    init();
    glutMainLoop();
    return 0;
}
