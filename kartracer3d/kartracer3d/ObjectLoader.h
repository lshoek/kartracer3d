#pragma once
#include <list>
#include <windows.h>
#include <glut.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "texture.h"

using namespace std;

class Vec3f
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};
	Vec3f();
	Vec3f(Vec3f &other);
	Vec3f(float x, float y, float z);
	float& operator [](int);
};

class Vec2f
{
public:
	union
	{
		struct
		{
			float x, y;
		};
		float v[2];
	};
	Vec2f();
	Vec2f(float x, float y);
	Vec2f(Vec2f &other);
	float& operator [](int);
};

class ObjModel
{
private:
	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord;
	};

	class Face
	{
	public:
		list<Vertex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		bool hasTexture;
		texture* tex;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		list<Face> faces;
	};


	vector<::Vec3f>	vertices;
	vector<::Vec3f>	normals;
	vector<::Vec2f>	texcoords;
	vector<ObjGroup*> groups;
	vector<MaterialInfo*> materials;

	void loadMaterialFile(std::string fileName, std::string dirName);
	

public:
	ObjModel(std::string filename);
	~ObjModel(void);
	void draw();
	string replace_str(string str, string toReplace, string replacement);
	vector<string> split_str(string str, string sep);
};


