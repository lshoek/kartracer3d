#include <string>
#include <iostream>
#include <vector>
#include <glut.h>
#include "game.h"
#include "font.h"
#include "texture.h"
#include "player.h"
#include "camera.h"

using namespace std;

game* game_g = NULL;
GLuint char_list, current_texture;
pair<texture*, texture*> crs_textures[6];
texture classicfnt_texture{ "resources/classicfnt32.png", GL_NEAREST };
camera cam;
bool key_up, key_down, key_left, key_right;
float ORG[3] = { 0, 0, 0 }, XP[3] = { 1, 0, 0 }, YP[3] = { 0, 1, 0 }, ZP[3] = { 0, 0, 1 };
int lastFrameTime = 0, txtrindex = 0;

void idleFunc()
{
	game_g->update();
}

void displayFunc()
{
	game_g->draw();
}

void keyDown(unsigned char key, int x, int y)
{
	game_g->kDown(key, x, y);
}

void keyUp(unsigned char key, int x, int y)
{
	game_g->kUp(key, x, y);
}

game::game()
{
	game_g = this;
	glEnable(GL_DEPTH_TEST); //Instead of glutInit
	glutInitWindowSize(SCRN_WIDTH, SCRN_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Realimaze");

	//Construct displaylists
	font classicfnt{ "resources/classicfnt32.fnt" };
	char_list = glGenLists(256);
	for (int i = 0; i < 256; i++)
	{
		if (classicfnt.charmap.find(i) == classicfnt.charmap.end())
			continue;
		glyph &g = classicfnt.charmap[i];
		float cx = g.x / 128.0f;
		float cy = g.y / 128.0f;
		float sizex = g.width / 128.0f;
		float sizey = g.height / 128.0f;

		glNewList(char_list + i, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(cx, cy);					glVertex2d(0, g.height);
		glTexCoord2f(cx, cy + sizey);			glVertex2d(0, 0);
		glTexCoord2f(cx + sizex, cy + sizey);	glVertex2d(g.width, 0);
		glTexCoord2f(cx + sizex, cy);			glVertex2d(g.width, g.height);
		glEnd();
		glTranslatef(g.xadvance, 0, 0);
		glEndList();
	}

	//Player
	player *p = new player();
	p->x = 0;
	p->y = 0;
	p->z = 0;
	p->rotation = 0;

	//Init crs_textures
	init_crs_textures();

	//Register callbacks
	glutIdleFunc(&idleFunc);
	glutDisplayFunc(&displayFunc);
	glutKeyboardFunc(&keyDown);
	glutKeyboardUpFunc(&keyUp);
	glutMainLoop();
}

game::~game()
{}

void game::init_crs_textures()
{
	crs_textures[0] = (pair<texture*, texture*>((new texture("resources/donutplainsgba_bg.png", GL_NEAREST)), (new texture("resources/donutplainsgba_track.png", GL_NEAREST))));
	crs_textures[1] = (pair<texture*, texture*>((new texture("resources/mariocircuit_bg.png", GL_NEAREST)), (new texture("resources/mariocircuit_track.png", GL_NEAREST))));
	crs_textures[2] = (pair<texture*, texture*>((new texture("resources/ghostvalley_bg.png", GL_NEAREST)), (new texture("resources/ghostvalley_track.png", GL_NEAREST))));
	crs_textures[3] = (pair<texture*, texture*>((new texture("resources/luigicircuit_bg.png", GL_NEAREST)), (new texture("resources/luigicircuit_track.png", GL_NEAREST))));
	crs_textures[4] = (pair<texture*, texture*>((new texture("resources/castle_bg.png", GL_NEAREST)), (new texture("resources/mariocircuit1_track.png", GL_NEAREST))));
	crs_textures[5] = (pair<texture*, texture*>((new texture("resources/chocoisland2_bg.png", GL_NEAREST)), (new texture("resources/chocoisland2_track.png", GL_NEAREST))));
}
void game::update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	cam.passTimeFac((time - lastFrameTime) / 1000.0);
	lastFrameTime = time;

	if (key_up)
		cam.move();
	if (key_left)
		cam.rotateYaw(-1);
	if (key_right)
		cam.rotateYaw(1);
	glutPostRedisplay();
}

void game::draw()
{
	glViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20, SCRN_WIDTH / (float)SCRN_HEIGHT, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.refresh();
	drawStage(0, 0, 0);
	glDisable(GL_DEPTH_TEST);
	drawAxes();

	//Orthogonal
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glOrtho(0, SCRN_WIDTH, 0, SCRN_HEIGHT, -1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawText("opengl 3d graphics demo", 10, 10, 2);

	drawText(cam.getVars(), 10, 20, 2);
	glutSwapBuffers();
}

void game::drawText(const string text, const GLfloat x, const GLfloat y, const int scale)
{
	current_texture = classicfnt_texture.getTextureId();
	glBindTexture(GL_TEXTURE_2D, current_texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(x, y, 0);
	glScalef(scale, scale, 1);
	glColor4f(0, 0, 0, 1);
	glListBase(char_list);
	glPushMatrix();
	glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
	glPopMatrix();
	glTranslatef(1, 1, 0);
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
	glPopMatrix();
}

void game::drawStage(GLfloat idx, GLfloat idy, GLfloat idz)
{
	glPushMatrix();
	glTranslatef(idx, idy, idz);
	glScalef(200, 20, 200);
	glEnable(GL_TEXTURE_2D);

	//track
	current_texture = crs_textures[txtrindex].second->getTextureId();
	glBindTexture(GL_TEXTURE_2D, current_texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(1, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(1, 0, 1);
	glTexCoord2f(1, 0); glVertex3f(0, 0, 1);
	glEnd();

	//bg
	current_texture = crs_textures[txtrindex].first->getTextureId();
	glBindTexture(GL_TEXTURE_2D, current_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(0, 2, 0);
	glTexCoord2f(1, 0); glVertex3f(1, 2, 0);
	glTexCoord2f(1, 1); glVertex3f(1, 0, 0);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(0, 2, 0);
	glTexCoord2f(1, 0); glVertex3f(0, 2, 1);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 1);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(1, 2, 0);
	glTexCoord2f(1, 0); glVertex3f(1, 2, 1);
	glTexCoord2f(1, 1); glVertex3f(1, 0, 1);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(1, 0, 1);
	glTexCoord2f(0, 0); glVertex3f(1, 2, 1);
	glTexCoord2f(1, 0); glVertex3f(0, 2, 1);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 1);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void game::drawAxes()
{
	glEnable(GL_COLOR);
	glPushMatrix();
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(10, 0, 0); // X (red)
	glVertex3fv(ORG);
	glVertex3fv(XP);
	glColor3f(0, 10, 0); // Y (green)
	glVertex3fv(ORG);
	glVertex3fv(YP);
	glColor3f(0, 0, 10); // Z (blue)
	glVertex3fv(ORG);
	glVertex3fv(ZP);
	glEnd();
	glPopMatrix();
}

void game::kDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 32:
		key_up = true;
		break;
	case 40:
		key_down = true;
		break;
	case 'a':
		key_left = true;
		break;
	case 'd':
		key_right = true;
		break;
	case 'c':
		if (txtrindex < 5) txtrindex++; else txtrindex = 0;
		break;
	case 27:
		exit(0);
		break;
	}
}

void game::kUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 32:
		key_up = false;
		break;
	case 40:
		key_down = false;
		break;
	case 'a':
		key_left = false;
		break;
	case 'd':
		key_right = false;
		break;
	case 27:
		exit(0);
		break;
	}
}