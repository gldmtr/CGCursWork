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
	Primitive(SceneNode* parent, Json::Value val)
	{
		ID = val["ID"].asInt();
		Selected = false;
		Position = Vector3f(val["Position"].asString());
		Size = Vector3f(val["Size"].asString());
		_type = val["PrimitiveType"].asInt();
		_wire = val["Wireframe"].asInt();
		_color = Vector3f(val["Color"].asString());
		_size = 1;
		Parent = parent;
		Json::ValueIterator iter = val["Childs"].begin();
		while (iter != val["Childs"].end())
		{
			Childs.push_back(new Primitive(this, *iter));
			iter++;
		}
	};

	~Primitive()
	{
	}

	virtual void Draw(bool pick = false)
	{
		bool wire = _wire;
		if (pick)
		{
			glLoadName(ID);
			wire = false;
		}
		glPushMatrix();
		/*glTranslatef(Position.x(), Position.y(), Position.z());
		glRotatef(Angles.z(), 0, 0, 1);
		glRotatef(Angles.y(), 0, 1, 0);
		glRotatef(Angles.x(), 1, 0, 0);*/
		ApplyTransform();
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
		for (int i = 0; i < Childs.size(); i++)
		{
			Childs[i]->Draw(pick);
		}
	}

	Json::Value Serialize(bool root = true) 
	{
		if (Parent != NULL && root)
		{
			return Json::Value(-1);
		}
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
			childs.append(Childs[i]->Serialize(false));
		}
		first["Childs"] = childs;
		return first;
	}

	Vector3f GetAbsoluteCoordinates()
	{
		Vector3f coord = Vector3f(0,0,0);
		SceneNode* cur = this;
		coord += Position;
		while (cur->Parent != NULL)
		{
			coord += cur->Parent->Position;
			cur = cur->Parent;
		}
		return coord;
	}

	Vector3f GetAbsoluteAngle()
	{
		Vector3f angle = Vector3f(0,0,0);
		SceneNode* cur = this;
		if (cur->Parent == NULL)
			angle += Angles;
		while (cur->Parent != NULL)
		{
			angle += cur->Angles;
			cur = cur->Parent;
		}
		return angle;
	}

	void ApplyTransform()
	{
		vector<SceneNode*> hierarhy;
		SceneNode* cur = this;
		hierarhy.push_back(cur);
		while (cur->Parent != NULL)
		{
			hierarhy.push_back(cur->Parent);
			cur = cur->Parent;
		}
		for (int i = hierarhy.size() - 1; i >= 0; i--)
		{
			glTranslatef(hierarhy[i]->Position.x(), hierarhy[i]->Position.y(), hierarhy[i]->Position.z());
			glRotatef(hierarhy[i]->Angles.z(), 0, 0, 1);
			glRotatef(hierarhy[i]->Angles.y(), 0, 1, 0);
			glRotatef(hierarhy[i]->Angles.x(), 1, 0, 0);
		}

	}

	SceneNode* GetSelected()
	{
		if (Selected)
			return this;
		for (int i = 0; i < Childs.size(); i++)
		{
			SceneNode* sel = NULL;
			if ((sel = Childs[i]->GetSelected()) != NULL)
				return sel;
		}
		return NULL;
	}
	
	SceneNode* GetByID(int id)
	{
		if (ID == id)
			return this;
		for (int i = 0; i < Childs.size(); i++)
		{
			SceneNode* sel = NULL;
			if ((sel = Childs[i]->GetByID(id)) != NULL)
				return sel;
		}
		return NULL;
	}

protected:
	int _type;
	bool _wire;
	float _size;
	Vector3f _color;
};