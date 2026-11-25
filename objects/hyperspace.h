#ifndef HYPERSPACE_H
#define HYPERSPACE_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#include "../models/asteroids.h"

#ifndef PI
#define PI 3.14159265f
#endif

#define STAR_COUNT 1300

typedef struct {
    int   useFree;  // 1: 消失点に縛られない自由座標, 0: 消失点基準（少しズレあり）
    float angle;    // 中心軸まわりの角度 [rad]
    float radius;   // 中心軸からの距離ma
    float offsetX;  // 星ごとの基準Xズレ
    float offsetY;  // 星ごとの基準Yズレ
    float freeX;    // 自由座標X
    float freeY;    // 自由座標Y
    float z;        // Z座標（奥の -Z から手前の +Z へ流す）
    float speed;    // 手前へ進む速さ
    float length;   // 光棒の長さ
    float bright;   // 明るさ
} HyperspaceStar;

static HyperspaceStar stars[STAR_COUNT];
static int hyperspaceEnabled = 1;

static int isDrawStars = 1;

// 視点正面（camX = 0.825f, camY = 0.35f 付近）に合わせた基準座標
static const float STAR_CENTER_X = 0.825f;
static const float STAR_CENTER_Y = 0.35f;
static const float STAR_CENTER_Z = 5.2f;
static const float COCKPIT_SCALE = 0.005f;

static int hyperspaceActive = 0; // 1: ハイパースペース再生中, 0: 待機中
static int totalFrame = 0;

void startHyperspace(void) {
    if(hyperspaceActive) return;

    hyperspaceEnabled = 1;
    hyperspaceActive = 1;
    totalFrame = 0;
    glutPostRedisplay();
}

int isHyperspaceActive(void) {
    return hyperspaceActive;
}

int hasDrawStars(){
    return isDrawStars;
}

static float randFloat(float minVal, float maxVal) {
    return minVal + (maxVal - minVal) * ((float)rand() / (float)RAND_MAX);
}

static int isInCockpit(float wx, float wy, float wz, float s) {
    float minX = -160.0f * s;
    float maxX =  490.0f * s;
    float minY = -320.0f * s;
    float maxY =  215.0f * s;
    float minZ =  -20.0f * s;
    float maxZ = 1440.0f * s;

    return (wx >= minX && wx <= maxX) &&
           (wy >= minY && wy <= maxY) &&
           (wz >= minZ && wz <= maxZ);
}

static int segmentHitsCockpit(float x, float y, float zFront, float zBack, float s) {
    const int steps = 8;
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / (float)steps;
        float z = zBack + (zFront - zBack) * t;
        if (isInCockpit(x, y, z, s)) {
            return 1;
        }
    }
    return 0;
}

static void resetStar(HyperspaceStar *st, int randomZ) {
    st->useFree = (randFloat(0.0f, 1.0f) < 0.40f) ? 1 : 0;

    st->angle   = randFloat(0.0f, 2.0f * (float)PI);
    st->radius  = randFloat(2.5f, 5.5f);
    st->offsetX = randFloat(-1.8f, 1.8f);
    st->offsetY = randFloat(-1.5f, 1.5f);

    // カメラ前後・周辺にも広がるよう少し広めに配置
    st->freeX   = randFloat(-7.0f, 8.5f);
    st->freeY   = randFloat(-5.0f, 5.5f);

    if (randomZ) {
        st->z = randFloat(-45.0f, 18.0f);
    } else {
        st->z = (randFloat(0.0f, 1.0f) < 0.5f)
                ? randFloat(-10.0f, 15.0f)
                : randFloat(-50.0f, 0.0f);
    }

    st->speed  = randFloat(0.60f, 1.65f);
    st->length = 3.0f * randFloat(1.2f, 4.5f) + 15.0f;
    st->bright = randFloat(0.5f, 1.0f);
}

static void getStarXY(const HyperspaceStar *st, float *outX, float *outY) {
    if (st->useFree) {
        *outX = st->freeX;
        *outY = st->freeY;
    } else {
        *outX = (STAR_CENTER_X + st->offsetX) + cosf(st->angle) * st->radius;
        *outY = (STAR_CENTER_Y + st->offsetY) + sinf(st->angle) * st->radius;
    }
}

void initHyperspace(void) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < STAR_COUNT; i++) {
        resetStar(&stars[i], 1);
    }
}

void updateHyperspace(float speedScale) {
    if (!hyperspaceEnabled || speedScale <= 0.0f) return;

    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i].z += stars[i].speed * speedScale;
        if (stars[i].z > 25.0f) {
            resetStar(&stars[i], 0);
        }
    }
}

void drawLight(float cRate, int isEnd) {
    if (!hyperspaceEnabled) return;

    GLboolean wasLight = glIsEnabled(GL_LIGHTING);
    GLboolean wasTex2D = glIsEnabled(GL_TEXTURE_2D);
    GLboolean wasBlend = glIsEnabled(GL_BLEND);

    if (wasLight) glDisable(GL_LIGHTING);
    if (wasTex2D) glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    // 1パス目：少し太めの光のにじみ
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < STAR_COUNT; i++) {
        float x, y;
        getStarXY(&stars[i], &x, &y);

        float zFront, zBack;
        if(!isEnd){
            zFront = stars[i].z - stars[i].length * (1.0f - cRate);
            zBack  = stars[i].z - stars[i].length;
        } else {
            zFront = stars[i].z;
            zBack  = stars[i].z - stars[i].length * cRate;
        }

        if (segmentHitsCockpit(x, y, zFront, zBack, COCKPIT_SCALE)) continue;

        float b = stars[i].bright * 0.65f;
        glColor4f(0.45f * b, 0.75f * b, 1.2f * b, 0.55f * b);
        glVertex3f(x, y, zFront);

        glColor4f(0.15f * b, 0.45f * b, 1.0f * b, 0.0f);
        glVertex3f(x, y, zBack);
    }
    glEnd();

    // 2パス目：明るい芯（先端を強調）
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for (int i = 0; i < STAR_COUNT; i++) {
        float x, y;
        getStarXY(&stars[i], &x, &y);

        float zFront, zBack;
        if(!isEnd){
            zFront = stars[i].z - stars[i].length * (1.0f - cRate);
            zBack  = stars[i].z - stars[i].length;
        } else {
            zFront = stars[i].z;
            zBack  = stars[i].z - stars[i].length * cRate;
        }

        if (segmentHitsCockpit(x, y, zFront, zBack, COCKPIT_SCALE)) continue;

        float b = stars[i].bright;
        glColor4f(0.95f * b, 1.15f * b, 1.4f * b, b);
        glVertex3f(x, y, zFront);

        glColor4f(0.35f * b, 0.65f * b, 1.2f * b, 0.12f * b);
        glVertex3f(x, y, zBack);
    }
    glEnd();

    glDepthMask(GL_TRUE);
    if (!wasBlend) glDisable(GL_BLEND);
    if (wasTex2D) glEnable(GL_TEXTURE_2D);
    if (wasLight) glEnable(GL_LIGHTING);
}

void drawHyperspace(void) {
    if (!hyperspaceEnabled || !hyperspaceActive) return;

    int a = 5;
    int b = 8;
    int c = 1;

    int durA = 30 * a; // 加速
    int durB = 30 * b; // 定速
    int durC = 30 * c; // 減速
    int totalDur = durA + durB + durC;

    int isIntit = 0;

    if (totalFrame < durA) {
        float ratio = (float)totalFrame / (float)durA;
        float curve = ratio * ratio;
        updateHyperspace(curve);
        drawLight(curve, 0);

        if(ratio > 0.15) isDrawStars = 0;
    }
    else if (totalFrame < durA + durB) {       
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        updateHyperspace(1.0f);
        drawLight(1.0f, 0);
    }
    else if (totalFrame < totalDur) {
        if(!isIntit){
            initStars();
            initRandomPlanets((Vec3){STAR_CENTER_X, STAR_CENTER_Y, STAR_CENTER_Z}, 0.005f);
            initRandomAsteroids((Vec3){STAR_CENTER_X, STAR_CENTER_Y, STAR_CENTER_Z}, 0.005f);
            isIntit = 1;
        }
        int f = totalFrame - (durA + durB);
        float ratio = 1.0f - ((float)f / (float)durC);
        if (ratio < 0.0f) ratio = 0.0f;
        float curve = ratio * ratio;
        updateHyperspace(curve);
        drawLight(curve, 1);
    }
    else {
        isDrawStars = 1;
        hyperspaceActive = 0;
        glutPostRedisplay();
        return;
    }

    totalFrame++;
    glutPostRedisplay();
}

#endif