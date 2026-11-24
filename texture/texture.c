#include <stdio.h>
#include <stdlib.h>
#include "texture.h"

#define TEXTURE_WIDTH  512
#define TEXTURE_HEIGHT 512

GLuint texid_canopy = 0;
GLuint texid_control_center0 = 0;
GLuint texid_sidein = 0;
GLuint texid_sideout0 = 0;
GLuint texid_sideout1 = 0;
GLuint texid_sideout2 = 0;
GLuint texid_column = 0;

// 指定された画像ファイルをロードする関数
GLuint loadTexture(const char* filename)
{
    unsigned char* image;
    FILE* fp;
    GLuint textureID;

    /* 画像データを格納するメモリを確保 */
    image = (unsigned char*)malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT * 3);

    if (image == NULL) {
        return 0;
    }

    /* RAWファイルを開く */
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("load fail\n");
        free(image);
        return 0;
    }

    size_t expected = (size_t)TEXTURE_WIDTH * TEXTURE_HEIGHT * 3;
    size_t readBytes = fread(image, 1, expected, fp);
    if (readBytes != expected) {
        fprintf(stderr, "Size mismatch (%s): got %zu bytes, expected %zu bytes\n",
                filename, readBytes, expected);
        fclose(fp);
        free(image);
        return 0;
    }

    fclose(fp);

    /* テクスチャIDを生成 */
    glGenTextures(1, &textureID);

    /* 使用するテクスチャを選択 */
    glBindTexture(GL_TEXTURE_2D, textureID);

    /* テクスチャの設定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* 画像データをOpenGLに登録 */
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        TEXTURE_WIDTH,
        TEXTURE_HEIGHT,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image
    );

    /* 読み込みに使ったメモリを解放 */
    free(image);

    return textureID;
}

void initTextures(void)
{
    texid_canopy = loadTexture(
        "texture/textures/metal_frame4.raw"
    );

    texid_control_center0 = loadTexture(
        "texture/textures/center0.raw"
    );

    texid_sidein = loadTexture(
        "texture/textures/side_in.raw"
    );

    texid_sideout0 = loadTexture(
        "texture/textures/side_out0.raw"
    );

    texid_sideout1 = loadTexture(
        "texture/textures/side_out1.raw"
    );

    texid_sideout2 = loadTexture(
        "texture/textures/side_out2.raw"
    );

    texid_column = loadTexture(
        "texture/textures/side_column.raw"
    );
}