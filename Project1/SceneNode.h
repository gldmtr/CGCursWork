#pragma once

#include <vector>
using namespace std;

#include "Vector3.h"

static int maxId = 0;

int GetID();

class SceneNode
{
public:
	SceneNode()
	{
		ID = GetID();
		Selected = false;
		Position = Vector3f(0,0,0);
	};
	~SceneNode()
	{
	};

	virtual void Draw(bool pick = false) = 0;

	int ID;
	bool Selected;
	SceneNode* Parent;
	vector<SceneNode*> Childs;
	Vector3f Position;
};

int GetID()
{
	return ++maxId;
}

void FlushID()
{
	maxId = 0;
}