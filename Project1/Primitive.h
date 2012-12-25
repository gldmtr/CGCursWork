#pragma once
#include <gl/freeglut.h>

#include "SceneNode.h"

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

private:
	int _type;
	bool _wire;
	float _size;
	Vector3f _color;
};