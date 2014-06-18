#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "math.h"
#include "font.h"
#include "texture.h"
#include "camera.h"
#include "drawobj.h"
#include "ObjectLoader.h"
#include <glut.h>
#define PI 3.14159265359
#define SCRN_WIDTH	1366
#define SCRN_HEIGHT	720

class game
{
public:
	game();
	~game();
	void init_textures();
	void update();
	void draw();
	void drawText(const string, const GLfloat, const GLfloat, const int);
	void drawBillboards(vector<drawobj*>);
	void drawPlayer(GLfloat idx, GLfloat idy, int width, int height);
	void drawCar(GLfloat idx, GLfloat idy, GLfloat idz);
	void drawStage(GLfloat, GLfloat, GLfloat);
	void drawAxes(); 
	void drawLine(GLfloat ax, GLfloat ay, GLfloat az, GLfloat bx, GLfloat by, GLfloat bz);
	void kDown(unsigned char key, int x, int y);
	void kUp(unsigned char key, int x, int y);
};

