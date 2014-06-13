#include <Windows.h>
#include <glut.h>
#include <string>
#include <iostream>
#include <map>
#include "stb_image.h"
#include "texture.h"
#include "glyph.h"

using namespace std;

texture::texture(const string fname)
{
	filename = fname.c_str();
	stbi_uc* img = stbi_load(filename, &width, &height, NULL, 4);
	image = img;
	textureId = 0;
	cout << "> " << filename << " loaded" << endl;
}

texture::texture(const string fname, const GLenum txfltr)
{
	filename = fname.c_str();
	txfiltering = txfltr;
	stbi_uc* img = stbi_load(filename, &width, &height, NULL, 4);
	image = img;
	textureId = 0;
	cout << "> " << filename << " loaded" << endl;
}

texture::~texture()
{
}

GLuint texture::getTextureId()
{
	if (textureId == 0)
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, txfiltering);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, txfiltering);
		glTexImage2D(GL_TEXTURE_2D,
			0,					//level
			GL_RGBA,			//internal format
			width,				//width
			height,				//height
			0,					//border
			GL_RGBA,			//data format
			GL_UNSIGNED_BYTE,	//data type
			image);				//data
	}
	return textureId;
}
