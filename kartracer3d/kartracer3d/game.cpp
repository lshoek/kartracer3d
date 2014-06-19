#include "game.h"
using namespace std;

game* game_g = NULL;
ObjModel* objm;
GLuint char_list, current_texture;
pair<texture*, texture*> crs_textures[6];
texture* player_textures[3];
texture* other_textures[4];
texture classicfnt_texture{ "resources/classicfnt32.png", GL_NEAREST };
texture water_texture{ "resources/water_ug.png", GL_NEAREST };
vector<drawobj*> billBoards;
camera cam;
bool key_up, key_down, key_left, key_right, lighting=true, drawmesh=true;
float ORG[3] = { 0, 0, 0 }, XP[3] = { 1, 0, 0 }, YP[3] = { 0, 1, 0 }, ZP[3] = { 0, 0, 1 }, car_rotation = 0;
int lastFrameTime = 0, txtrindex = 0, player_index = 2, last_player_index = 2;

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

	//Init crs_textures
	init_textures();

	//Init drawobjs
	for (int i = 0; i <= 10; i++)
	{
		billBoards.push_back(new drawobj(rand() % 300, 0, rand() % 300, other_textures[0], 2, 2));
		billBoards.push_back(new drawobj(rand() % 300, 0, rand() % 300, other_textures[1], 10, 10));
		billBoards.push_back(new drawobj(rand() % 300, 0, rand() % 300, other_textures[2], 6, 10));
		billBoards.push_back(new drawobj(rand() % 300, 0, rand() % 300, other_textures[3], 6, 10));
	}

	player_textures[0] = new texture("resources/player/player-1.png", GL_NEAREST);
	player_textures[1] = new texture("resources/player/player0.png", GL_NEAREST);
	player_textures[2] = new texture("resources/player/player1.png", GL_NEAREST);

	objm = new ObjModel("resources/models/simplecar.obj");

	//LIGHTING
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	//Register callbacks
	glutIdleFunc(&idleFunc);
	glutDisplayFunc(&displayFunc);
	glutKeyboardFunc(&keyDown);
	glutKeyboardUpFunc(&keyUp);
	glutMainLoop();
}

game::~game()
{}

void game::init_textures()
{
	GLenum filtering = GL_NEAREST;
	crs_textures[0] = (pair<texture*, texture*>((new texture("resources/donutplainsgba_bg.png", filtering)), (new texture("resources/donutplainsgba_track.png", filtering))));
	crs_textures[1] = (pair<texture*, texture*>((new texture("resources/mariocircuit_bg.png", filtering)), (new texture("resources/mariocircuit_track.png", filtering))));
	crs_textures[2] = (pair<texture*, texture*>((new texture("resources/ghostvalley_bg.png", filtering)), (new texture("resources/ghostvalley_track.png", filtering))));
	crs_textures[3] = (pair<texture*, texture*>((new texture("resources/luigicircuit_bg.png", filtering)), (new texture("resources/luigicircuit_track.png", filtering))));
	crs_textures[4] = (pair<texture*, texture*>((new texture("resources/castle_bg.png", filtering)), (new texture("resources/peachcircuit_track.png", filtering))));
	crs_textures[5] = (pair<texture*, texture*>((new texture("resources/chocoisland2_bg.png", filtering)), (new texture("resources/chocoisland2_track.png", filtering))));

	other_textures[0] = new texture("resources/banana.png", filtering);
	other_textures[1] = new texture("resources/tree1.png", filtering);
	other_textures[2] = new texture("resources/tree3.png", filtering);
	other_textures[3] = new texture("resources/tree4.png", filtering);
}

void game::update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	cam.passTimeFac((time - lastFrameTime) / 1000.0);
	lastFrameTime = time;
	player_index = 1;

	if (key_up)
		cam.move();
	if (key_left) 
	{
		cam.rotateYaw(-1);
		if (last_player_index <= 1)
			player_index = 0;
	}
	else if (key_right) 
	{
		cam.rotateYaw(1); 
		if (last_player_index >= 1)
			player_index = 2;
	}
	last_player_index = player_index;
	glutPostRedisplay();
}

void game::draw()
{
	glViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);

	//clearcolors
	if (txtrindex == 0) //donutplains
		glClearColor(255 / 255.0, 191 / 255.0, 80 / 255.0, 1);
	else if (txtrindex == 1) //mariocircuit
		glClearColor(160 / 255.0, 240 / 255.0, 255 / 255.0, 1);
	else if (txtrindex == 2) //ghostvalley
		glClearColor(128 / 255.0, 144 / 255.0, 176 / 255.0, 1);
	else if (txtrindex == 3) //luigicircuit
		glClearColor(176 / 255.0, 191 / 255.0, 1, 1);
	else if (txtrindex == 4) //castle
		glClearColor(15 / 255.0, 159 / 255.0, 255 / 255.0, 1);
	else if (txtrindex == 5) //chocoisland
		glClearColor(223 / 255.0, 0, 96 / 255.0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	//Perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	gluPerspective(70, SCRN_WIDTH / (float)SCRN_HEIGHT, 1, 1000); //fov default 60

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//LIGHTING
	GLfloat LightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);

	GLfloat LightModelAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat MaterialSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat MaterialEmission[] = { 0, 0, 0, 1.0f };
	glLightModelfv(GL_AMBIENT, LightModelAmbient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, MaterialEmission);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Cam (gluLookat)
	cam.refresh();

	//Stage & Car
	drawStage(0, 0, 0);
	if (drawmesh)
		drawCar(80, 1, 80);

	//Obstacles
	if (txtrindex != 2)
		drawBillboards(billBoards);
	glDisable(GL_LIGHTING); 

	//Orthogonal
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glOrtho(0, SCRN_WIDTH, 0, SCRN_HEIGHT, -1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawPlayer(SCRN_WIDTH / 2, SCRN_HEIGHT*0.25, 100, 100);
	drawText("opengl 3d graphics demo", 10, 10, 2);
	drawText(cam.getVars(), 10, 20, 2);
	glutSwapBuffers();
}

void game::drawText(const string text, const GLfloat x, const GLfloat y, const int scale)
{
	current_texture = classicfnt_texture.getTextureId();
	glBindTexture(GL_TEXTURE_2D, current_texture);
	glEnable(GL_TEXTURE_2D);

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

void game::drawPlayer(GLfloat idx, GLfloat idy, int width, int height)
{
	current_texture = player_textures[player_index]->getTextureId();
	glBindTexture(GL_TEXTURE_2D, current_texture);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(idx-width, idy+height, 0);
	glScalef(2.0, -2.0, 1.0);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex2d(0, 0);
	glTexCoord2f(0, 1);		glVertex2d(0, height);
	glTexCoord2f(1, 1);		glVertex2d(width, height);
	glTexCoord2f(1, 0);		glVertex2d(width, 0);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void game::drawCar(GLfloat idx, GLfloat idy, GLfloat idz)
{
	glPushMatrix();
	glTranslatef(idx, idy, idz);

	glTranslatef(50, 0, -5);
	glRotatef(-car_rotation, 0, 1, 0);
	glTranslatef(-50, 0, 5);
	glScalef(5, 5, 5);

	glDisable(GL_TEXTURE_2D);
	objm->draw();
	glPopMatrix();

	car_rotation+= 0.2;
	if (car_rotation >= 360)
		car_rotation = 0;
}

void game::drawStage(GLfloat idx, GLfloat idy, GLfloat idz)
{
	glPushMatrix();
	glTranslatef(idx, idy, idz);
	if (txtrindex == 1 || txtrindex == 2 || txtrindex == 4)
		glScalef(300, 40, 300);
	else if (txtrindex == 3)
		glScalef(400, 60, 400);
	else
		glScalef(200, 30, 200);

	if (lighting)
		glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	if (txtrindex == 0)
	{
		//add water
		glBindTexture(GL_TEXTURE_2D, water_texture.getTextureId());
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-0.5, -0.075, -0.5);
		glTexCoord2f(0, 1); glVertex3f(1, -0.075, -0.5);
		glTexCoord2f(1, 1); glVertex3f(1, -0.075, 2.0);
		glTexCoord2f(1, 0); glVertex3f(-0.5, -0.075, 2.0);
		glEnd();
	}

	//track
	current_texture = crs_textures[txtrindex].second->getTextureId();
	glBindTexture(GL_TEXTURE_2D, current_texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(1, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(1, 0, 1);
	glTexCoord2f(1, 0); glVertex3f(0, 0, 1);
	glEnd();

	if (txtrindex == 2)
		glTranslatef(0, -0.4, 0);
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
	glTexCoord2f(0, 1); glVertex3f(0, 0, 1);
	glTexCoord2f(0, 0);	glVertex3f(0, 2, 1);
	glTexCoord2f(1, 0);	glVertex3f(0, 2, 0);
	glTexCoord2f(1, 1);	glVertex3f(0, 0, 0);
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

void game::drawLine(GLfloat ax, GLfloat ay, GLfloat az, GLfloat bx, GLfloat by, GLfloat bz)
{
	glEnable(GL_COLOR);
	glPushMatrix();
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); 
	glVertex3f(ax, ay, az);
	glVertex3f(bx, by, bz);
	glEnd();
	glPopMatrix();
	glDisable(GL_COLOR);
}

void game::drawBillboards(vector<drawobj*> vec)
{
	for (drawobj* d : vec)
		d->draw();
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
	case 'v':
		lighting = !lighting;
		break;
	case 'x':
		drawmesh = !drawmesh;
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