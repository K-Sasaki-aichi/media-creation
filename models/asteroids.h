
#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../vector.h"
#include "../texture/texture.h"
#include "../objects/asteroid.h"

#ifndef PI
#define PI 3.14159265f
#endif

#define MAX_ACTIVE_ASTEROIDS 35

/* 小惑星の配置範囲 */
static float g_outerAsteroidRadius = 100.0f;

/* 表示する小惑星の数 */
static int g_activeAsteroidCount = 20;

/* 小惑星の情報 */
typedef struct {
    Vec3 position;      /* ワールド座標 */
    float radius;       /* 大きさ */
    float rotX;         /* X軸回転 */
    float rotY;         /* Y軸回転 */
    float rotZ;         /* Z軸回転 */
    unsigned int seed;  /* 形状を決める乱数シード */
} AsteroidInstance;

static AsteroidInstance g_asteroids[MAX_ACTIVE_ASTEROIDS];
static int g_isAsteroidsInitialized = 0;


/* 0～1の乱数 */
static float asteroidRandFloat01(void)
{
    return (float)rand() / (float)RAND_MAX;
}

static void initRandomAsteroids(Vec3 center, float canopyScale)
{
    int i;

    /* 乱数シード初期化（初回のみ） */
    static int seeded = 0;

    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    /* 表示数を制限 */
    if (g_activeAsteroidCount > MAX_ACTIVE_ASTEROIDS) {
        g_activeAsteroidCount = MAX_ACTIVE_ASTEROIDS;
    }

    if (g_activeAsteroidCount < 0) {
        g_activeAsteroidCount = 0;
    }

    /* キャノピーから小惑星までの安全距離 */
    float canopySafeRadius = 850.0f * canopyScale;

    for (i = 0; i < g_activeAsteroidCount; i++) {

        /* 小惑星の大きさ */
        float radius = 2.0f + asteroidRandFloat01() * 5.0f;

        /* 配置可能な距離 */
        float rMin = canopySafeRadius + radius;
        float rMax = g_outerAsteroidRadius - radius;

        if (rMax <= rMin) {
            rMax = rMin + 10.0f;
        }

        /* 球殻内に体積一様分布する距離 */
        float u = asteroidRandFloat01();

        float r = cbrtf(
            u * (rMax * rMax * rMax -
                 rMin * rMin * rMin) +
                 rMin * rMin * rMin
        );

        /* ランダムな方向 */
        float theta = 2.0f * PI * asteroidRandFloat01();

        float cosPhi = 2.0f * asteroidRandFloat01() - 1.0f;

        float sinPhi = sqrtf(fmaxf(0.0f, 1.0f - cosPhi * cosPhi));

        g_asteroids[i].position.x = center.x + r * sinPhi * cosf(theta);

        g_asteroids[i].position.y =center.y + r * cosPhi;

        g_asteroids[i].position.z = center.z + r * sinPhi * sinf(theta);

        g_asteroids[i].radius = radius;

        g_asteroids[i].rotX = asteroidRandFloat01() * 360.0f;
        g_asteroids[i].rotY = asteroidRandFloat01() * 360.0f;
        g_asteroids[i].rotZ = asteroidRandFloat01() * 360.0f;

        g_asteroids[i].seed = (unsigned int)(i + 1) * 12345u;
    }

    g_isAsteroidsInitialized = 1;
}

static void drawAsteroids(Vec3 center, float canopyScale)
{
    int i;

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    /* 小惑星のテクスチャ */
    glBindTexture(GL_TEXTURE_2D, texid_asteroid);

    for (i = 0; i < g_activeAsteroidCount; i++) {

        glPushMatrix();

        /* ランダム配置された座標へ移動 */
        glTranslatef(
            g_asteroids[i].position.x,
            g_asteroids[i].position.y,
            g_asteroids[i].position.z
        );

        /* ランダムな回転 */
        glRotatef(g_asteroids[i].rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(g_asteroids[i].rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(g_asteroids[i].rotZ, 0.0f, 0.0f, 1.0f);

        /* 小惑星を描画 */
        drawAsteroid(
            g_asteroids[i].radius,
            g_asteroids[i].seed
        );

        glPopMatrix();
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib();
}

#endif
