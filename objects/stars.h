#ifndef STARS_H
#define STARS_H

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#ifndef PI
#define PI 3.14159265f
#endif

#define STAR_NUM 1400

typedef struct {
    float x;          // 星のX座標
    float y;          // 星のY座標
    float z;          // 星のZ座標
    float size;       // 星の大きさ（ポイントサイズ）
    float brightness; // 光の強さ（0.0f 〜 1.0f）
} BgStar;

static BgStar bgStars[STAR_NUM];

// 星空の中心基準（コックピット／カメラ周辺に合わせると全方位に囲みやすい）
static const float STAR_ORIGIN_X = 0.825f;
static const float STAR_ORIGIN_Y = 0.35f;
static const float STAR_ORIGIN_Z = 5.2f;

static float randRange(float minVal, float maxVal) {
    return minVal + (maxVal - minVal) * ((float)rand() / (float)RAND_MAX);
}

void initStars(void) {
    const float minR = 85.0f;
    const float maxR = 95.0f;

    for (int i = 0; i < STAR_NUM; i++) {
        float r = randRange(minR, maxR);

        // 球面上に偏りなく分布させるための角度計算
        float theta = randRange(0.0f, 2.0f * (float)PI);
        float cosPhi = randRange(-1.0f, 1.0f);
        float sinPhi = sqrtf(1.0f - cosPhi * cosPhi);
        if (sinPhi < 0.0f) sinPhi = 0.0f;

        float dx = r * sinPhi * cosf(theta);
        float dy = r * cosPhi;
        float dz = r * sinPhi * sinf(theta);

        bgStars[i].x = STAR_ORIGIN_X + dx;
        bgStars[i].y = STAR_ORIGIN_Y + dy;
        bgStars[i].z = STAR_ORIGIN_Z + dz;

        bgStars[i].size = randRange(1.2f, 5.0f);
        bgStars[i].brightness = randRange(0.35f, 1.0f);
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

    for (int i = 0; i < STAR_NUM; i++) {
        float b = bgStars[i].brightness;

        glPointSize(bgStars[i].size);
        glBegin(GL_POINTS);
        // ごくわずかに青白〜白系の星色
        glColor4f(0.88f * b, 0.94f * b, 1.0f * b, b);
        glVertex3f(bgStars[i].x, bgStars[i].y, bgStars[i].z);
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