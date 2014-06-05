#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <glut.h>
#include "font.h"
#include "texture.h"
#define SCRN_WIDTH	1280
#define SCRN_HEIGHT	720

class game
{
public:
	game();
	~game();
	void init_crs_textures();
	void update();
	void draw();
	void drawText(const string, const GLfloat, const GLfloat, const int);
	void drawStage(GLfloat, GLfloat, GLfloat);
	void drawAxes();
	void kDown(unsigned char key, int x, int y);
	void kUp(unsigned char key, int x, int y);
};

