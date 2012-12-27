#pragma once
#include "Primitive.h"

class PrimitiveModel : Primitive
{
public:
	PrimitiveModel(SceneNode* parent, Json::Value val)
		: Primitive(parent, val)
	{
	};

	vector<pair<Vector3f, Vector3f>> Bones;
	Vector3f Joint;

	void Draw(bool pick = false)
	{
		bool wire = _wire;
		if (pick)
			wire = false;
		glPushMatrix();
		glTranslatef(Position.x(), Position.y(), Position.z());
		glRotatef(Angles.z(),0,0,1);
		glRotatef(Angles.x(),1,0,0);
		glRotatef(Angles.y(),0,1,0);
		glScalef(Size.x(), Size.y(), Size.z());
		glColor3fv(&_color[0]);
		switch (_type)
		{
		case SPHERE:
			if (!wire)
				glutSolidSphere(_size, 16, 16);
			else
				glutWireSphere(_size, 16, 16);
			break;
		case CUBE:
			if (!wire)
				glutSolidCube(_size);
			else
				glutWireCube(_size);
			break;
		default:
			break;
		}
		glPopMatrix();
	}
};