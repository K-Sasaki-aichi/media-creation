
#ifndef ASTEROID_H
#define ASTEROID_H

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265f
#endif

#define ASTEROID_SLICES 32
#define ASTEROID_STACKS 24

/* 乱数生成 */
static unsigned int asteroidRandom(unsigned int *seed)
{
    *seed = *seed * 1664525u + 1013904223u;
    return *seed;
}

static float asteroidRandomFloat(unsigned int *seed)
{
    return (float)(asteroidRandom(seed) % 10000) / 10000.0f;
}

/* ベクトルの外積 */
static void asteroidCross(
    const float a[3],
    const float b[3],
    float result[3])
{
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

/* ベクトルの正規化 */
static void asteroidNormalize(float v[3])
{
    float len = sqrtf(
        v[0] * v[0] +
        v[1] * v[1] +
        v[2] * v[2]
    );

    if (len > 0.00001f)
    {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
}

/* 小惑星の頂点を生成 */
static void createAsteroidVertex(
    float vertex[ASTEROID_STACKS + 1]
                 [ASTEROID_SLICES + 1][3],
    float radius,
    unsigned int seed)
{
    float largeNoise[ASTEROID_STACKS + 1]
                    [ASTEROID_SLICES + 1];

    float smallNoise[ASTEROID_STACKS + 1]
                    [ASTEROID_SLICES + 1];

    unsigned int localSeed = seed;

    int i, j;

    /* 大きな凹凸と小さな凹凸を生成 */
    for (i = 0; i <= ASTEROID_STACKS; i++)
    {
        for (j = 0; j <= ASTEROID_SLICES; j++)
        {
            largeNoise[i][j] =
                asteroidRandomFloat(&localSeed);

            smallNoise[i][j] =
                asteroidRandomFloat(&localSeed);
        }
    }

    /* 頂点座標を計算 */
    for (i = 0; i <= ASTEROID_STACKS; i++)
    {
        float theta = PI * (float)i / ASTEROID_STACKS;

        for (j = 0; j <= ASTEROID_SLICES; j++)
        {
            float phi = 2.0f * PI * (float)j / ASTEROID_SLICES;

            /* 隣接頂点の乱数を平均して滑らかにする */
            float noise = largeNoise[i][j];

            if (i > 0)
                noise += largeNoise[i - 1][j];

            if (i < ASTEROID_STACKS)
                noise += largeNoise[i + 1][j];

            if (j > 0)
                noise += largeNoise[i][j - 1];

            if (j < ASTEROID_SLICES)
                noise += largeNoise[i][j + 1];

            noise /= 5.0f;

            /* 大きな凹凸 + 小さな凹凸 */
            float variation =
                0.88f +
                (noise - 0.5f) * 0.55f +
                (smallNoise[i][j] - 0.5f) * 0.12f;

            float r = radius * variation;

            vertex[i][j][0] =
                r * sinf(theta) * cosf(phi);

            vertex[i][j][1] =
                r * cosf(theta);

            vertex[i][j][2] =
                r * sinf(theta) * sinf(phi);
        }
    }
}

/* 小惑星描画 */
void drawAsteroid(float radius, unsigned int seed)
{
    float vertex[ASTEROID_STACKS + 1]
                [ASTEROID_SLICES + 1][3];

    int i, j;

    createAsteroidVertex(vertex, radius, seed);

    glBegin(GL_TRIANGLES);

    for (i = 0; i < ASTEROID_STACKS; i++)
    {
        for (j = 0; j < ASTEROID_SLICES; j++)
        {
            int ni = i + 1;
            int nj = j + 1;

            float u0 = (float)j / ASTEROID_SLICES;
            float u1 = (float)(j + 1) / ASTEROID_SLICES;

            float v0 = 1.0f - (float)i / ASTEROID_STACKS;
            float v1 = 1.0f - (float)(i + 1) / ASTEROID_STACKS;

            float a[3], b[3], normal[3];

            /* 1つ目の三角形の法線 */
            for (int k = 0; k < 3; k++)
            {
                a[k] = vertex[ni][j][k] - vertex[i][j][k];
                b[k] = vertex[i][nj][k] - vertex[i][j][k];
            }

            asteroidCross(a, b, normal);
            asteroidNormalize(normal);

            glNormal3fv(normal);

            glTexCoord2f(u0, v0);
            glVertex3fv(vertex[i][j]);

            glTexCoord2f(u0, v1);
            glVertex3fv(vertex[ni][j]);

            glTexCoord2f(u1, v0);
            glVertex3fv(vertex[i][nj]);

            /* 2つ目の三角形の法線 */
            for (int k = 0; k < 3; k++)
            {
                a[k] = vertex[ni][j][k] - vertex[i][nj][k];
                b[k] = vertex[ni][nj][k] - vertex[i][nj][k];
            }

            asteroidCross(a, b, normal);
            asteroidNormalize(normal);

            glNormal3fv(normal);

            glTexCoord2f(u1, v0);
            glVertex3fv(vertex[i][nj]);

            glTexCoord2f(u0, v1);
            glVertex3fv(vertex[ni][j]);

            glTexCoord2f(u1, v1);
            glVertex3fv(vertex[ni][nj]);
        }
    }

    glEnd();
}

#endif
