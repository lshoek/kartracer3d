 #include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <glut.h>
#include "camera.h"

using namespace std;

#define M_PI 3.14159265358979323846
#define M_PI_2 3.14159265358979323846*2

void camera::init()
{
	m_yaw = 0.0;
	m_pitch = 0.0;
	setPos(35, 2, 12);
}

void camera::refresh()
{
	//camera parameter according to Riegl's co-ordinate system
	//x/y for flat, z for height

	//lookat vectors
	m_lx = cos(m_yaw) * cos(m_pitch);
	m_ly = sin(m_pitch);
	m_lz = sin(m_yaw) * cos(m_pitch);

	//position vectors
	float lx = cos(m_yaw)*cos(m_pitch);
	float ly = sin(m_pitch);
	float lz = sin(m_yaw)*cos(m_pitch);
	m_x += spd*tfac*lx;
	m_y += spd*tfac*ly;
	m_z += spd*tfac*lz;

	//speed
	if (spd > 0.001 && !isMoving) spd *= spd_dec;

	if (prev_spd > spd && spd < 0.01) spd = 0;
	else if (spd > max_spd) spd = max_spd;
	prev_spd = spd;

	//magic
	gluLookAt(m_x, m_y, m_z, m_x + m_lx, m_y + m_ly, m_z + m_lz, 0.0, 1.0, 0.0);

	//rotation dec
	if (rotation_spd_r > rotation_acc-0.001) rotation_spd_r *= spd_dec;
	else rotation_spd_r = 0;
	if (rotation_spd_l > rotation_acc-0.001) rotation_spd_l *= spd_dec;
	else rotation_spd_l = 0;

	//rotation acc
	if (rotation_spd_r > rotation_spd_l)
		rotation_spd = rotation_spd_r - rotation_spd_l;
	else
		rotation_spd = (rotation_spd_l - rotation_spd_r)*-1;

	//rotation update
	m_yaw += rotation_spd*(spd / max_spd)*tfac;

	//set movement back to false
	isMoving = false;
}

void camera::setPos(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void camera::getPos(float &x, float &y, float &z)
{
	x = m_x;
	y = m_y;
	z = m_z;
}

void camera::getDirectionVector(float &x, float &y, float &z)
{
	x = m_lx;
	y = m_ly;
	z = m_lz;
}

string camera::getVars()
{
	stringstream strs;
	strs << "spd=" << spd << " rotation_spd=" << rotation_spd << endl;
	return strs.str();
}

void camera::move()
{
	isMoving = true;
	if (spd < max_spd)
		spd += spd_acc*(1-spd/max_spd);
}

void camera::fly(float incr)
{
	m_y += incr;
}

void camera::rotateYaw(float angle)
{
	if (angle > 0 && rotation_spd_r < rotation_max_spd)
		rotation_spd_r += rotation_acc;
	if (angle < 0 && rotation_spd_r < rotation_max_spd)
		rotation_spd_l += rotation_acc;
}

void camera::rotatePitch(float angle)
{
	const float limit = 89.0 * M_PI / 180.0;

	m_pitch += angle;

	if (m_pitch < -limit)
		m_pitch = -limit;

	if (m_pitch > limit)
		m_pitch = limit;
}

void camera::passTimeFac(float tf)
{
	tfac = tf;
}

void camera::setYaw(float angle)
{
	m_yaw = angle;
}

void camera::setPitch(float angle)
{
	m_pitch = angle;
}
