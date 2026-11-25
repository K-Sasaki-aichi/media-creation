#ifndef HYPERSPACE_H
#define HYPERSPACE_H

#include <time.h>
#include <math.h>
#include <GL/glut.h>

#include <stdio.h>

#ifndef PI
#define PI 3.14159265f
#endif
#define STAR_COUNT 1200

typedef struct {
    float angle;   // 中心軸まわりの角度 [rad]
    float radius;  // 中心軸からの距離
    float z;       // Z座標（奥の -Z から手前の +Z へ流す）
    float speed;   // 手前へ進む速さ
    float length;  // 光棒の長さ
    float bright;  // 明るさ
} HyperspaceStar;

static HyperspaceStar stars[STAR_COUNT];
static int hyperspaceEnabled = 1;

// 中心軸の基準座標（現在のカメラXY付近に合わせると正面から流れやすい）
static float starCenterX = 0.825f;
static float starCenterY = 0.35f;

static int isInCockpit(float wx, float wy, float wz, float s) {
    float minX = -155.0f * s;
    float maxX =  485.0f * s;
    float minY = -320.0f * s;
    float maxY =  210.0f * s;

    float minZ =   -20.0f * s; // 少し手前〜奥まで余裕を持たせる
    float maxZ = 1400.0f * s;

    return (wx >= minX && wx <= maxX) &&
           (wy >= minY && wy <= maxY) &&
           (wz >= minZ && wz <= maxZ);
}

static int segmentHitsCockpit(float x, float y, float zFront, float zBack, float s) {
    const int steps = 8; // 線分を8分割してチェック
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / (float)steps;
        float z = zBack + (zFront - zBack) * t;
        if (isInCockpit(x, y, z, s)) {
            return 1;
        }
    }
    return 0;
}

static float randFloat(float minVal, float maxVal) {
    return minVal + (maxVal - minVal) * ((float)rand() / (float)RAND_MAX);
}

static int isInCockpitXY(float wx, float wy, float s) {
    // キャノピー・コックピット周辺のX/Y範囲（必要に応じて微調整）
    float minX = -155.0f * s;
    float maxX =  485.0f * s;
    float minY = -320.0f * s;
    float maxY =  210.0f * s;

    return (wx >= minX && wx <= maxX) &&
           (wy >= minY && wy <= maxY);
}

static void resetStar(HyperspaceStar *st, int randomZ) {

    st->angle = randFloat(0.0f, 2.0f * (float)PI);

    // 中心のすぐ近くは少し避けつつ、円形〜リング状に分布させる
    st->radius = randFloat(1.5f, 4.5f);
    st->z = randomZ ? randFloat(-45.0f, 3.0f) : randFloat(-50.0f, -25.0f);
    st->speed = randFloat(0.60f, 1.65f);
    st->length = 3*randFloat(1.2f, 4.5f) + 15.0;
    st->bright = randFloat(0.4f, 1.0f);

}

void initHyperspace(void) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < STAR_COUNT; i++) {
        resetStar(&stars[i], 1);
    }
}

void updateHyperspace(float speedScale) {
    if (speedScale <= 0.0f) return;
    if (!hyperspaceEnabled) return;

    for (int i = 0; i < STAR_COUNT; i++) {
        // 速度倍率に応じて奥(-Z)から手前(+Z)へ移動
        stars[i].z += stars[i].speed * speedScale;

        // カメラより手前（camZ 付近）を通り過ぎたら奥へ戻す
        if (stars[i].z > 15) {
            resetStar(&stars[i], 0);
        }
    }
}

void drawLight(float cRate) {
    if (!hyperspaceEnabled) return;

    GLboolean wasLight   = glIsEnabled(GL_LIGHTING);
    GLboolean wasTex2D   = glIsEnabled(GL_TEXTURE_2D);
    GLboolean wasBlend   = glIsEnabled(GL_BLEND);

    if (wasLight) glDisable(GL_LIGHTING);
    if (wasTex2D) glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // 加算合成で光を強く見せる
    glDepthMask(GL_FALSE);              // 深度バッファを汚さない

    glLineWidth(2.0f);
    glBegin(GL_LINES);

    float s = 0.005f;

    for (int i = 0; i < STAR_COUNT; i++) {
        float x = starCenterX + cosf(stars[i].angle) * stars[i].radius;
        float y = starCenterY + sinf(stars[i].angle) * stars[i].radius;

        // 手前側（明るい先端）と、奥側（伸びた光棒の尻尾）
        float zFront = stars[i].z - stars[i].length*(1-cRate);
        float zBack  = stars[i].z - stars[i].length;

        if(segmentHitsCockpit(x, y, zFront, zBack, s)) continue;

        // 少し青白いハイパースペースカラー
        glColor4f(0.65f * stars[i].bright,
                  0.85f * stars[i].bright,
                  1.0f  * stars[i].bright,
                  stars[i].bright);
        glVertex3f(x, y, zFront);

        glColor4f(0.2f, 0.5f, 1.0f, 0.0f);
        glVertex3f(x, y, zBack);
    }

    glEnd();

    glDepthMask(GL_TRUE);
    if (!wasBlend) glDisable(GL_BLEND);
    if (wasTex2D) glEnable(GL_TEXTURE_2D);
    if (wasLight) glEnable(GL_LIGHTING);
}

void drawHyperspace(void)
{
    static int totalFrame = 0;

    int a = 8;
    int b = 16;
    float c = 2;

    int durA = 30 * a; // 加速：180フレーム
    int durB = 30 * b; // 定速：900フレーム
    int durC = 30 * c; // 減速：30フレーム
    int totalDur = durA + durB + durC;

    if (totalFrame < durA) {
        // 加速フェーズ：0 → 1 へ滑らかに加速＋光棒が伸びる
        float ratio = (float)totalFrame / (float)durA;
        float curve = ratio * ratio;
        updateHyperspace(curve);
        drawLight(curve);
    }
    else if (totalFrame < durA + durB) {
        // 定速フェーズ：加速の終わり（1.0f）とそのまま同じ速度で接続
        updateHyperspace(1.0f);
        drawLight(1.0f);
    }
    else if (totalFrame < totalDur) {
        // 減速フェーズ：1 → 0 へ滑らかに減速＋光棒が縮む
        int f = totalFrame - (durA + durB);
        float ratio = 1.0f - ((float)f / (float)durC);
        if (ratio < 0.0f) ratio = 0.0f;
        float curve = ratio * ratio;
        updateHyperspace(curve);
        drawLight(curve);
    }
    else {
        totalFrame = 0;
        glutPostRedisplay();
        return;
    }

    totalFrame++;
    glutPostRedisplay();
}
// void drawHyperspace(void)
// {
//     static int totalFrame = 0;

//     int a = 6;
//     int b = 30;
//     int c = 1;

//     int durA = 30 * a; // 加速：30フレーム
//     int durB = 30 * b; // 定速：150フレーム
//     int durC = 30 * c; // 減速：30フレーム
//     int totalDur = durA + durB + durC;

//     float maxAdd = 1.0f;

//     if (totalFrame < durA) {
//         // 加速フェーズ
//         float ratio = (float)totalFrame / (float)durA;
//         updateHyperspace(0.05);
//         drawLight(ratio*ratio*ratio);
//     }
//     else if (totalFrame < durA + durB) {
//         // 定速フェーズ
//         updateHyperspace(1.0);
//         drawLight(1.0f);
//     }
//     else if (totalFrame < totalDur) {
//         // 減速フェーズ
//         int f = totalFrame - (durA + durB);
//         float ratio = 1.0f - ((float)f / (float)durC);
//         updateHyperspace(0);
//         drawLight(ratio*ratio);
//     }
//     else {
//         totalFrame = 0;
//         return;
//     }

//     totalFrame++;
//     glutPostRedisplay();
// }

#endif