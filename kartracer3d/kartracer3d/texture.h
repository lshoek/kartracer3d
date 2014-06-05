#pragma once
#include <Windows.h>
#include <glut.h>
#include <string>
#include <iostream>
#include <map>
#include "stb_image.h"
#include "glyph.h"

using namespace std;

class texture
{
private:
	unsigned char* image;
	const char* filename;
	GLenum txfiltering = GL_LINEAR;
	int width;
	int height;
	GLuint textureId;

public:
	texture(const string);
	texture(const string, GLenum);
	~texture();
	GLuint getTextureId();
};

