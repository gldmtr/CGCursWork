#pragma once
#include "SceneNode.h"
#include "Primitive.h"
#include "Device.h"

class Scene
{
public:
	Scene()
	{
		CameraPosition = Vector3f(0,10,0);
		CameraRotation = Vector3f(30,0,0);
		IsTranslation = false;
		IsNewObject = false;
		NewId = -1;
	};

	~Scene()
	{
	};

	void PushObjects()
	{
		glInitNames();
		glPushName(0);
		for (int i = 0; i < this->Nodes.size(); i++)
		{
			glLoadName(this->Nodes[i]->ID);
			this->Nodes[i]->Draw(true);
		}
	}

	int GetSelectBufferObject ( int x, int y )
	{
		GLuint selBuffer[40] = {0};
		int	viewport[4], Found = 0;

		glSelectBuffer ( 40, selBuffer );
		glGetIntegerv  ( GL_VIEWPORT, viewport );
		glMatrixMode   ( GL_PROJECTION );	
		glPushMatrix();
			glRenderMode ( GL_SELECT );
			glLoadIdentity();
			gluPickMatrix ( x, viewport[3] - y, 1, 1, viewport );
			gluPerspective(device->FovY,(GLfloat)device->Widht/(GLfloat)(device->Height+1),0.1f,100.0f);
			glMatrixMode ( GL_MODELVIEW );
				PushObjects();
				Found = glRenderMode ( GL_RENDER );
			glMatrixMode ( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode ( GL_MODELVIEW );

		if ( Found > 0 )
		{
			GLuint Depth    = selBuffer[1];
			int  selObject = selBuffer[3];
			for ( int i = 1; i < Found; i++ )
			{
				if ( selBuffer[(i * 4) + 1] < Depth )
				{
					Depth    = selBuffer[(i * 4) + 1];
					selObject = selBuffer[(i * 4) + 3];
				}
			}
			cout << selObject <<  endl;
			return selObject;
		}

		return -1;											
	}

	SceneNode* GetObjectByID(int id)
	{
		for (int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->ID == id)
			{
				return Nodes[i];
			}
		}
		return NULL;
	}

	Vector3f GetPoint(int x, int y)
	{
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;
 
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);
 
		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
 
		gluUnProject(winX, winY, zPoint, modelview, projection, viewport, &posX, &posY, &posZ);
		
		return Vector3f(posX, posY, posZ);
	}

	Vector3f GetSelectLine(int x, int y)
	{
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;
		GLdouble posX1, posY1, posZ1;
		glLoadIdentity();
 
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);
 
		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
		winZ = 0;
 
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		winZ = -1;

		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX1, &posY1, &posZ1);

		Vector3f line = Vector3f(posX - posX1, posY - posY1, posZ - posZ1);
		line.normalize();
		
		return line;
	}

	SceneNode* GetSelectedNode()
	{
		for (int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->Selected)
				return Nodes[i];
		}
		return NULL;
	}

	void DisableSelection()
	{
		for (int i = 0; i < Nodes.size(); i++)
		{
			Nodes[i]->Selected = false;
		}
	}

	int AddNewNode(SceneNode* node)
	{
		IsNewObject = true;
		Nodes.push_back(node);
		NewId = node->ID;
		Translation = Vector3<bool>(true, true, true);
		return NewId;
	}

	void Save(char* filename)
	{
		Json::StyledWriter writer;
		ofstream out(filename, fstream::out);
		Json::Value root(Json::arrayValue);
		for (int i = 0; i < Nodes.size(); i++)
		{
			root.append(Nodes[i]->Serialize());
		}
		out << writer.write(root);
		out.close();
	}

	void Load(string filename)
	{
		Json::Reader reader;
		Json::Value root;

		ifstream in(filename, ifstream::in);
		char buf[512*1024] = {0};
		string str;
		while (in.getline(buf, 512*1024))
		{
			str += buf;
			ZeroMemory(buf, 512*1024);
		}
		cout << str;
		bool parsingSuccessful = reader.parse(str, root, false);
		if ( !parsingSuccessful )
		{
			// report to the user the failure and their locations in the document.
			std::cout  << reader.getFormatedErrorMessages()
				   << "\n";
		}
		Json::ValueIterator iter = root.begin();

		Nodes.clear();

		while (iter != root.end())
		{
			Json::Value single = (*iter);
			if (single["ClassType"] == "Primitive")
			{
				SceneNode* newNode = new Primitive(single);
				Nodes.push_back(newNode);
			}
			iter ++;
		}
		in.close();
	}

	vector<SceneNode*> Nodes;
	Vector3<bool> Translation;
	Vector3f CursorPos;
	float translationKoef;
	Vector3f CameraPosition, CameraRotation;
	bool IsTranslation;
	bool IsNewObject;
	int NewId;
	float zPoint;
	int SelectedAxis;
	bool SkipTransform;
};

Scene *scene = new Scene();