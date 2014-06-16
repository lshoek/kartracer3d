#include "drawobj.h"

drawobj::drawobj(GLfloat objPosX, GLfloat objPosY, GLfloat objPosZ, texture *txtr, GLfloat w, GLfloat h)
{
	xPos = objPosX;
	yPos = objPosY;
	zPos = objPosZ;
	t = txtr;
	width = w;
	height = h;
}

drawobj::drawobj()
{
}

drawobj::~drawobj()
{
}

void drawobj::draw()
{
	float modelview[16];
	glPushMatrix();
	glTranslatef(xPos + width / 2, yPos, zPos + width / 2);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	//de eerste 3 linksboven worden eenheidsmatrix
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j)
				modelview[i * 4 + j] = 1; //treat the 1-dimensional array as a 4x4 array
			else
				modelview[i * 4 + j] = 0;
		}
	}
	glLoadMatrixf(modelview);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t->getTextureId());

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glNormal3f(0, 0, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glNormal3f(0, 0, 1); glVertex3f(0, height, 0);
	glTexCoord2f(1, 0); glNormal3f(0, 0, 1); glVertex3f(width, height, 0);
	glTexCoord2f(1, 1); glNormal3f(0, 0, 1); glVertex3f(width, 0, 0);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}