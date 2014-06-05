#include <string>
#include <sstream>
#include "glyph.h"

using namespace std;

glyph::glyph()
{
}

glyph::~glyph()
{
}

string glyph::tostring()
{
	stringstream ss;
	ss << "id=" << id << " x=" << x << " y=" << y << " width=" << width << " height=" << height << " yoffset=" << yoffset << " xadvance=" << xadvance;
	return ss.str();
}
