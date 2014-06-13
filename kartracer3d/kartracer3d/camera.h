#pragma once
#include <stdio.h>
#include <math.h>
#include <string>
#include <sstream>
#include <glut.h>

using namespace std;
class camera
{
public:
	camera() { init(); }
	~camera(){}

	void init();
	void refresh();
	void setPos(float x, float y, float z);
	void getPos(float &x, float &y, float &z);
	string getVars();
	void getDirectionVector(float &x, float &y, float &z);
	void passTimeFac(float tf);
	void setYaw(float angle);
	void setPitch(float angle);

	void move();
	void fly(float incr);
	void rotateYaw(float angle);
	void rotatePitch(float angle);

   private:
	bool isMoving = false;
	float tfac;

	float spd = 0;
	float prev_spd;
	const float max_spd = 50;
	const float spd_acc = 0.1;
	const float spd_dec = 0.990;

	float rotation_spd_r = 0;
	float rotation_spd_l = 0;
	float rotation_spd = 0;
	const float rotation_max_spd = 2.5;
	const float rotation_acc = 0.04;

	float m_x, m_y, m_z;
	float m_lx, m_ly, m_lz;
	float m_yaw, m_pitch;
};