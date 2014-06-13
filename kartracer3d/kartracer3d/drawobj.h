#pragma once
#include "texture.h"
#include <glut.h>

class drawobj
{
public:
	GLfloat xPos, yPos, zPos, width, height;
	texture* t;

	drawobj(GLfloat, GLfloat, GLfloat, texture*, GLfloat, GLfloat);
	drawobj();
	~drawobj();
	void draw();
};

