#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include "glyph.h"

using namespace std;

class font
{
private:
	void printFontData();
	string replace(string str, string toReplace, string replacement);
	vector<string> split(string str, string sep);

public:
	map<char, glyph> charmap;
	typedef map<char, glyph>::iterator cgmap;
	font(const string fname);
	font();
	~font();
};


