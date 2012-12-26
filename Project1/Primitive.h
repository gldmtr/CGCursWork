#pragma once
#include <gl/freeglut.h>
#include <string>

#include "SceneNode.h"

#define Tab(var, level) for (int i = 0; i < level; i++) \
							var += "\t";

using namespace std;

class Primitive : public SceneNode
{
public:
	Primitive(Vector3f position, int type, float size, Vector3f color, bool isWireframe = false, SceneNode* parent = 0)
	{
		_type = type;
		_wire = isWireframe;
		_size = size;
		Parent = parent;
		Position = position;
		_color = color;
	}

	~Primitive()
	{
	}

	void Draw(bool pick = false)
	{
		bool wire = _wire;
		if (pick)
			wire = false;
		glPushMatrix();
		glTranslatef(Position.x(), Position.y(), Position.z());
		glScalef(Size.x(), Size.y(), Size.z());
		glRotatef(Angles.x(),1,0,0);
		glRotatef(Angles.y(),0,1,0);
		glRotatef(Angles.z(),0,0,1);
		glColor3fv(&_color[0]);
		switch (_type)
		{
		case 0:
			if (!wire)
				glutSolidSphere(_size, 16, 16);
			else
				glutSolidCube(_size);
			break;
		default:
			break;
		}
		glPopMatrix();
	}

	void WriteToFile(ofstream* file, int level)
	{
		char buf[128] = {0};
		string out;
		Tab(out, level);
		out += "<SceneNode>\n";
			Tab(out, level+1);
			sprintf(buf, "<Type = Primitive />\n\0");
			out += buf;
			Tab(out, level+1);
			sprintf(buf, "<ID = %d />\n\0", ID);
			out += buf;
			Tab(out, level+1);
			sprintf(buf, "<Position = %4.2f %4.2f %4.2f />\n\0", Position.x(),  Position.y(), Position.z());
			out += buf;
			Tab(out, level+1);
			sprintf(buf, "<Size = %4.2f %4.2f %4.2f />\n\0", Size.x(),  Size.y(), Size.z());
			out += buf;
		Tab(out, level);
		out += "</SceneNode>\n";
		file->write(out.c_str(), out.length());
	}

private:
	int _type;
	bool _wire;
	float _size;
	Vector3f _color;
};