#pragma once
#include <string>
#include <sstream>

using namespace std;

class glyph
{
public:
	char id;
	int x, y, width, height, xoffset, yoffset, xadvance;
	glyph();
	~glyph();
	string tostring();
};

