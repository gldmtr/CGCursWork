#pragma once

#include <vector>
#include <fstream>
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
		Size = Vector3f(1,1,1);
	};
	~SceneNode()
	{
	};

	virtual void Draw(bool pick = false) = 0;
	virtual void WriteToFile(ofstream* file, int level) = 0;
	void Translate(Vector3f dimension);

	int ID;
	bool Selected;
	SceneNode* Parent;
	vector<SceneNode*> Childs;
	Vector3f Position;
	Vector3f Size;
};

void SceneNode::Translate(Vector3f dimension)
{
	Position += dimension;
}

int GetID()
{
	return ++maxId;
}

void FlushID()
{
	maxId = 0;
}