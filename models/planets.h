#ifndef PLANET_H
#define PLANET_H

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../vector.h"
#include "../texture/texture.h"
#include "../objects/sphere.h"

#ifndef PI
#define PI 3.14159265f
#endif


#define TOTAL_PLANET_TYPES 7      /* 用意されている惑星テクスチャの総数 */
#define MAX_ACTIVE_PLANETS 7      /* 同時に配置可能な最大数 */

/* 後から変更可能なグローバル変数 */
static int   g_activePlanetCount = 5;      /* 表示する惑星の数（デフォルト3つ） */
static float g_outerSphereRadius = 100.0f; /* 配置する外周球面の半径（デフォルト200） */

typedef struct {
    int   typeIndex;   /* 0〜5の惑星インデックス */
    Vec3  position;    /* ワールド座標 */
    float radius;      /* 惑星ごとの固定サイズ */
    float tiltAngle;   /* 自転軸の傾き */
} PlanetInstance;

static PlanetInstance g_planets[MAX_ACTIVE_PLANETS];
static int g_isPlanetsInitialized = 0;

/* 惑星インデックス(0〜5)に対応するテクスチャIDを取得 */
static GLuint getPlanetTexture(int index)
{
    switch (index) {
        case 0: return texid_planet1; // jupiter
        case 1: return texid_planet2; // neptune
        case 2: return texid_planet3; // saturn
        case 3: return texid_planet4; // venus_atmosphere
        case 4: return texid_planet5; // venus_surface
        case 5: return texid_planet6; // mercury
        case 6: return texid_planet7; // deth star
        default: return texid_planet1;
    }
}

/* 惑星ごとの固定サイズ（半径）を返す */
static float getPlanetRadius(int index)
{
    switch (index) {
        case 0: return 18.0f; // jupiter
        case 1: return 11.0f; // neptune
        case 2: return 15.0f; // saturn
        case 3: return 8.0f;  // venus_atmosphere
        case 4: return 8.0f;  // venus_surface
        case 5: return 5.0f;  // mercury
        case 6: return 25.0f;  // deth star
        default: return 10.0f;
    }
}

static float randFloat01(void)
{
    return (float)rand() / (float)RAND_MAX;
}

static void initRandomPlanets(Vec3 center, float canopyScale)
{
    int i, j, tmp;
    int pool[TOTAL_PLANET_TYPES] = { 0, 1, 2, 3, 4, 5 };

    /* 乱数シード初期化（初回のみ） */
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    /* 表示数が上限を超えないようクランプ */
    if (g_activePlanetCount > TOTAL_PLANET_TYPES) {
        g_activePlanetCount = TOTAL_PLANET_TYPES;
    }
    if (g_activePlanetCount < 0) {
        g_activePlanetCount = 0;
    }

    /* シャッフル */
    for (i = TOTAL_PLANET_TYPES - 1; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = pool[i];
        pool[i] = pool[j];
        pool[j] = tmp;
    }

    /* キャノピーの最遠部（中心から約800*s） + 安全マージン */
    float canopySafeRadius = 850.0f * canopyScale;

    /* 選ばれた惑星の座標とサイズを決定 */
    for (i = 0; i < g_activePlanetCount; i++) {
        int type = pool[i];
        float pRadius = getPlanetRadius(type);

        float rMin = canopySafeRadius + pRadius;
        float rMax = g_outerSphereRadius - pRadius;

        /* 万が一 rMin >= rMax になった場合の安全策 */
        if (rMax <= rMin) {
            rMax = rMin + 10.0f;    // if (!g_isPlanetsInitialized) {
    //     initRandomPlanets(center, canopyScale);
    // }
        }

        /* 球殻内に体積一様分布するようにランダム距離 r を算出 */
        float u = randFloat01();
        float r = cbrtf(u * (rMax * rMax * rMax - rMin * rMin * rMin) + (rMin * rMin * rMin));

        /* 球面上のランダムな方向（方位角 theta, 天頂角 phi） */
        float theta = 2.0f * PI * randFloat01();
        float cosPhi = 2.0f * randFloat01() - 1.0f;
        float sinPhi = sqrtf(fmaxf(0.0f, 1.0f - cosPhi * cosPhi));

        g_planets[i].typeIndex = type;
        g_planets[i].radius    = pRadius;
        g_planets[i].tiltAngle = (randFloat01() - 0.5f) * 30.0f; /* -15度〜+15度の傾き */

        g_planets[i].position.x = center.x + r * sinPhi * cosf(theta);
        g_planets[i].position.y = center.y + r * cosPhi;
        g_planets[i].position.z = center.z + r * sinPhi * sinf(theta);
    }

    g_isPlanetsInitialized = 1;
}

/* =========================================================
 * 惑星群の描画関数（display関数内で呼び出す）
 * ========================================================= */
static void drawPlanets(Vec3 center, float canopyScale)
{
    int i;

    /* 初回描画時に自動でランダム配置を計算 */
    // if (!g_isPlanetsInitialized) {
    //     initRandomPlanets(center, canopyScale);
    // }

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (i = 0; i < g_activePlanetCount; i++) {
        GLuint texId = getPlanetTexture(g_planets[i].typeIndex);

        glPushMatrix();

        /* ランダム配置された座標へ移動 */
        glTranslatef(
            g_planets[i].position.x,
            g_planets[i].position.y,
            g_planets[i].position.z
        );

        /* テクスチャの極（北極・南極）がY軸（上下）に来るように補正 */
        glRotatef(g_planets[i].tiltAngle, 0.0f, 0.0f, 1.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, texId);
        drawSphere(g_planets[i].radius, 36, 24);

        glPopMatrix();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopAttrib();
}

#endif