#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>

extern GLuint texid_canopy;
extern GLuint texid_control_center;
extern GLuint texid_control_center0;
extern GLuint texid_control_center1;
extern GLuint texid_sidein;
extern GLuint texid_sideout0;
extern GLuint texid_sideout1;
extern GLuint texid_sideout2;
extern GLuint texid_column;
extern GLuint texid_glass;
extern GLuint texid_plate;
extern GLuint texid_body;
extern GLuint texid_engine;

void initTextures(void);
//void deleteTextures(void);

#endif