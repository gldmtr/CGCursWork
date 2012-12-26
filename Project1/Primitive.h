#pragma once
#include <gl/freeglut.h>
#include <string>

#include "SceneNode.h"
#include "json\json.h"

#define Tab(var, level) for (int i = 0; i < level; i++) \
							var += "\t";
#define SPHERE 0
#define CUBE 1

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
	Primitive(Json::Value val)
	{
		ID = val["ID"].asInt();
		Selected = false;
		Position = Vector3f(val["Position"].asString());
		Size = Vector3f(val["Size"].asString());
		_type = val["PrimitiveType"].asInt();
		_wire = val["Wireframe"].asInt();
		_color = Vector3f(val["Color"].asString());
		_size = 1;
	};

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
		glRotatef(Angles.x(),1,0,0);
		glRotatef(Angles.y(),0,1,0);
		glRotatef(Angles.z(),0,0,1);
		glScalef(Size.x(), Size.y(), Size.z());
		glColor3fv(&_color[0]);
		switch (_type)
		{
		case 0:
			if (!wire)
				glutSolidSphere(_size, 16, 16);
			else
				glutWireCube(_size);
			break;
		default:
			break;
		}
		glPopMatrix();
	}

	Json::Value Serialize() 
	{
		Json::Value first(Json::objectValue);
		first["ID"] = ID;
		first["ClassType"] = "Primitive";
		first["PrimitiveType"] = _type;
		first["Wireframe"] = _wire;
		first["Position"] = Position.toString();
		first["Size"] = Size.toString();
		first["Color"] = _color.toString();
		first["Parent"] = Json::Value(Parent == NULL ? 0 : Parent->ID);
		Json::Value childs(Json::arrayValue);
		for (int i = 0; i < Childs.size(); i++)
		{
			childs.append(Childs[i]->Serialize());
		}
		first["Childs"] = childs;
		return first;
	}

private:
	int _type;
	bool _wire;
	float _size;
	Vector3f _color;
};