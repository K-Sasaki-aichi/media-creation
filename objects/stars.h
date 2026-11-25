#ifndef STARS_H
#define STARS_H

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#ifndef PI
#define PI 3.14159265f
#endif

#define STAR_NUM 2000

typedef struct {
    float x;
    float y;
    float z;
    float size;
    float brightness;

    float r;
    float g;
    float b;
} BgStar;

static BgStar bgStars[STAR_NUM];

// 星空の中心基準（コックピット／カメラ周辺に合わせると全方位に囲みやすい）
static const float STAR_ORIGIN_X = 0.825f;
static const float STAR_ORIGIN_Y = 0.35f;
static const float STAR_ORIGIN_Z = 5.2f;

static float randRange(float minVal, float maxVal) {
    return minVal + (maxVal - minVal) * ((float)rand() / (float)RAND_MAX);
}

void initStars(void)
{
    const float minR = 85.0f;
    const float maxR = 95.0f;

    for (int i = 0; i < STAR_NUM; i++) {

        float r = randRange(minR, maxR);

        float theta = randRange(0.0f, 2.0f * (float)PI);
        float cosPhi = randRange(-1.0f, 1.0f);
        float sinPhi = sqrtf(1.0f - cosPhi * cosPhi);

        float dx = r * sinPhi * cosf(theta);
        float dy = r * cosPhi;
        float dz = r * sinPhi * sinf(theta);

        bgStars[i].x = STAR_ORIGIN_X + dx;
        bgStars[i].y = STAR_ORIGIN_Y + dy;
        bgStars[i].z = STAR_ORIGIN_Z + dz;

        bgStars[i].size = randRange(1.2f, 6.0f);
        bgStars[i].brightness = randRange(0.45f, 1.0f);

        /* 星色をランダム化 */
        float type = randRange(0.0f, 1.0f);

        if (type < 0.55f) {
            /* 白 */
            bgStars[i].r = 1.0f;
            bgStars[i].g = 1.0f;
            bgStars[i].b = 1.0f;
        }
        else if (type < 0.75f) {
            /* 青白 */
            bgStars[i].r = 0.75f;
            bgStars[i].g = 0.85f;
            bgStars[i].b = 1.0f;
        }
        else if (type < 0.92f) {
            /* 黄白 */
            bgStars[i].r = 1.0f;
            bgStars[i].g = 0.95f;
            bgStars[i].b = 0.75f;
        }
        else {
            /* 赤系 */
            bgStars[i].r = 1.0f;
            bgStars[i].g = 0.7f;
            bgStars[i].b = 0.55f;
        }
    }
}
void drawStars(void) {
    GLboolean wasLight = glIsEnabled(GL_LIGHTING);
    GLboolean wasTex2D = glIsEnabled(GL_TEXTURE_2D);
    GLboolean wasBlend = glIsEnabled(GL_BLEND);
    GLboolean wasPointSmooth = glIsEnabled(GL_POINT_SMOOTH);

    GLfloat prevPointSize = 1.0f;
    glGetFloatv(GL_POINT_SIZE, &prevPointSize);

    if (wasLight) glDisable(GL_LIGHTING);
    if (wasTex2D) glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glDepthMask(GL_FALSE); // 背景の星で深度バッファを塞がない

    float t = glutGet(GLUT_ELAPSED_TIME) * 0.001f;

    for (int i = 0; i < STAR_NUM; i++) {

        /* 微弱な瞬き */
        float twinkle = 0.70f + 0.15f * sinf(t * 2.0f + i * 0.37f);

        float br =
            bgStars[i].brightness * twinkle;

        glPointSize(bgStars[i].size);

        glBegin(GL_POINTS);

        glColor4f(
            bgStars[i].r * br,
            bgStars[i].g * br,
            bgStars[i].b * br,
            br
        );

        glVertex3f(
            bgStars[i].x,
            bgStars[i].y,
            bgStars[i].z
        );

        glEnd();
    }

    // 状態を元に戻す
    glDepthMask(GL_TRUE);
    glPointSize(prevPointSize);

    if (!wasPointSmooth) glDisable(GL_POINT_SMOOTH);
    if (!wasBlend) glDisable(GL_BLEND);
    if (wasTex2D) glEnable(GL_TEXTURE_2D);
    if (wasLight) glEnable(GL_LIGHTING);
}

#endif