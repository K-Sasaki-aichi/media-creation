#include <GL/glut.h>
#include "../objects/cuboid.h"
#include "canopy.h"


#include <GL/glut.h>
#include "../objects/cuboid.h"

void drawCanopy(float s)
{
    // =====================================
    // 各部の座標
    // =====================================

    // 前方の頂点
    float frontZ = -1340.0f * s;
    float frontY = 170.0f * s;
    float frontW = 225.0f * s;

    // 中央断面
    float centerZ = 0.0f;
    float centerTopY = 360.0f * s;
    float centerBottomY = 0.0f;

    float centerTopW = 315.0f * s;
    float centerBottomW = 490.0f * s;

    // 後方：天板の終端
    float rearTopZ = 245.0f * s;
    float rearTopY = 360.0f * s;

    // 後方：尖った頂点
    float rearTipZ = 400.0f * s;
    float rearTipY = 210.0f * s;

    // 後方の幅
    float rearW = 272.5f * s;

    // 骨組みの太さ
    // 座標全体は s で縮小しているが、
    // 太さはワールド座標で指定する
    float t = 0.09f;


    // =====================================
    // 1. 前方部分
    // =====================================

    // 前方頂点から中央上端へ
    drawBeam(
        -frontW, frontY, frontZ,
        -centerTopW, centerTopY, centerZ, t
    );

    drawBeam(
        frontW, frontY, frontZ,
        centerTopW, centerTopY, centerZ, t
    );

    // 前方頂点から中央下端へ
    drawBeam(
        -frontW, frontY, frontZ,
        -centerBottomW, centerBottomY, centerZ, t
    );

    drawBeam(
        frontW, frontY, frontZ,
        centerBottomW, centerBottomY, centerZ, t
    );

    // 前方の横骨
    drawBeam(
        -frontW, frontY, frontZ,
        frontW, frontY, frontZ, t
    );


    // =====================================
    // 2. 中央断面
    // =====================================

    // 上側の横骨
    drawBeam(
        -centerTopW, centerTopY, centerZ,
        centerTopW, centerTopY, centerZ, t
    );

    // 下側の横骨
    drawBeam(
        -centerBottomW, centerBottomY, centerZ,
        centerBottomW, centerBottomY, centerZ, t
    );

    // 左右の側面骨
    drawBeam(
        -centerTopW, centerTopY, centerZ,
        -centerBottomW, centerBottomY, centerZ, t
    );

    drawBeam(
        centerTopW, centerTopY, centerZ,
        centerBottomW, centerBottomY, centerZ, t
    );


    // =====================================
    // 3. 中央から後方の天板終端
    // =====================================

    // 中央上端から後方上端へ
    drawBeam(
        -centerTopW, centerTopY, centerZ,
        -rearW, rearTopY, rearTopZ, t
    );

    drawBeam(
        centerTopW, centerTopY, centerZ,
        rearW, rearTopY, rearTopZ, t
    );


    // =====================================
    // 4. 後方の下側
    // =====================================

    // 中央下端から後方頂点へ
    drawBeam(
        -centerBottomW, centerBottomY, centerZ,
        -rearW, rearTipY, rearTipZ, t
    );

    drawBeam(
        centerBottomW, centerBottomY, centerZ,
        rearW, rearTipY, rearTipZ, t
    );


    // =====================================
    // 5. 天板終端から後方頂点へ
    // =====================================

    // 左側：高さ360から210へ下がる
    drawBeam(
        -rearW, rearTopY, rearTopZ,
        -rearW, rearTipY, rearTipZ, t
    );

    // 右側
    drawBeam(
        rearW, rearTopY, rearTopZ,
        rearW, rearTipY, rearTipZ, t
    );


    // =====================================
    // 6. 後方の横骨
    // =====================================

    // 天板終端の横骨
    drawBeam(
        -rearW, rearTopY, rearTopZ,
        rearW, rearTopY, rearTopZ, t
    );

    // 後方頂点の横骨
    drawBeam(
        -rearW, rearTipY, rearTipZ,
        rearW, rearTipY, rearTipZ, t
    );


    // =====================================
    // 7. 天板
    // =====================================

    glBegin(GL_QUADS);

    // 左中央
    glVertex3f(
        -centerTopW, centerTopY, centerZ
    );

    // 左後方
    glVertex3f(
        -rearW, rearTopY, rearTopZ
    );

    // 右後方
    glVertex3f(
        rearW, rearTopY, rearTopZ
    );

    // 右中央
    glVertex3f(
        centerTopW, centerTopY, centerZ
    );

    glEnd();
}
