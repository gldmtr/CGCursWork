#pragma once
#include "SceneNode.h"
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

	void PushAxis()
	{
		glInitNames();
		glPushName(0);
		Vector3f pos = GetSelectedNode()->Position;
		SceneNode* selected = GetSelectedNode();
		glPushMatrix();
			glTranslatef(selected->Position.x(), selected->Position.y(), selected->Position.z());
				glPushMatrix();
				glLoadName(1);
				glBegin(GL_LINES);
					glVertex3fv(&Vector3f(0,0,0)[0]);
					glVertex3fv(&Vector3f(2,0,0)[0]);
				glEnd();
				glTranslatef(1.5,0,0);
					glRotatef(90, 0, 1, 0);
					glutSolidCone(0.2, 0.7, 16, 16);
				glPopMatrix();

				glPushMatrix();
				glLoadName(2);
				glBegin(GL_LINES);
					glVertex3fv(&Vector3f(0,0,0)[0]);
					glVertex3fv(&Vector3f(0,2,0)[0]);
				glEnd();
				glTranslatef(0,1.5,0);
					glRotatef(90, -1, 0, 0);
					glutSolidCone(0.2, 0.7, 16, 16);
				glPopMatrix();

				glPushMatrix();
				glLoadName(3);
				glBegin(GL_LINES);
					glVertex3fv(&Vector3f(0,0,0)[0]);
					glVertex3fv(&Vector3f(0,0,2)[0]);
				glEnd();
				glTranslatef(0,0,1.5);
					glutSolidCone(0.2, 0.7, 16, 16);
				glPopMatrix();
			glPopMatrix();

		glLoadName(4);	// плоскость xy
			glBegin(GL_QUADS);
				glVertex3f(pos.x(), pos.y(), pos.z());
				glVertex3f(pos.x(), pos.y() + 1.5f, pos.z());
				glVertex3f(pos.x() + 1.5f, pos.y() + 1.5f, pos.z());
				glVertex3f(pos.x() + 1.5f, pos.y(), pos.z());
			glEnd();

		glLoadName(5);	// плоскость xz
			glBegin(GL_QUADS);
				glVertex3f(pos.x(), pos.y(), pos.z());
				glVertex3f(pos.x(), pos.y(), pos.z() + 1.5f);
				glVertex3f(pos.x() + 1.5f, pos.y(), pos.z() + 1.5f);
				glVertex3f(pos.x() + 1.5f, pos.y(), pos.z());
			glEnd();

		glLoadName(6);	// плоскость yz
			glBegin(GL_QUADS);
				glVertex3f(pos.x(), pos.y(), pos.z());
				glVertex3f(pos.x(), pos.y() + 1.5f, pos.z());
				glVertex3f(pos.x(), pos.y() + 1.5f, pos.z() + 1.5f);
				glVertex3f(pos.x(), pos.y(), pos.z() + 1.5f);
			glEnd();
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

	int GetSelectAxisObject( int x, int y )
	{
		if (GetSelectedNode() == NULL)
		{
			return -1;
		}
		GLuint selBuffer[12] = {0};
		int	viewport[4], Found = 0;

		glSelectBuffer ( 12, selBuffer );
		glGetIntegerv  ( GL_VIEWPORT, viewport );
		glMatrixMode   ( GL_PROJECTION );	
		glPushMatrix();
			glRenderMode ( GL_SELECT );
			glLoadIdentity();
			gluPickMatrix ( x-2, viewport[3] - y - 2, 4, 4, viewport );
			gluPerspective(device->FovY,(GLfloat)device->Widht/(GLfloat)(device->Height+1),0.1f,100.0f);
			glMatrixMode ( GL_MODELVIEW );
				PushAxis();
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
		GLdouble posX1, posY1, posZ1;\
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

	void DrawAxex()
	{
		SceneNode *selected;
		if ((selected = GetSelectedNode()) != NULL)
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glPushMatrix();
			glTranslatef(selected->Position.x(), selected->Position.y(), selected->Position.z());
				glPushMatrix();
				(SelectedAxis == 1)||(SelectedAxis == 4)||(SelectedAxis == 5) ? glColor3f(1,1,0) : glColor3f(1,0,0);
				glBegin(GL_LINES);
					glVertex3fv(&Vector3f(0,0,0)[0]);
					glVertex3fv(&Vector3f(2,0,0)[0]);
				glEnd();
				glTranslatef(1.5,0,0);
					glRotatef(90, 0, 1, 0);
					glutSolidCone(0.2, 0.7, 16, 16);
				glPopMatrix();

				glPushMatrix();
				(SelectedAxis == 2)||(SelectedAxis == 4)||(SelectedAxis == 6) ? glColor3f(1,1,0) : glColor3f(0,1,0);
				glBegin(GL_LINES);
					glVertex3fv(&Vector3f(0,0,0)[0]);
					glVertex3fv(&Vector3f(0,2,0)[0]);
				glEnd();
				glTranslatef(0,1.5,0);
					glRotatef(90, -1, 0, 0);
					glutSolidCone(0.2, 0.7, 16, 16);
				glPopMatrix();

				glPushMatrix();
				(SelectedAxis == 3)||(SelectedAxis == 5)||(SelectedAxis == 6) ? glColor3f(1,1,0) : glColor3f(0,0,1);
				glBegin(GL_LINES);
					glVertex3fv(&Vector3f(0,0,0)[0]);
					glVertex3fv(&Vector3f(0,0,2)[0]);
				glEnd();
				glTranslatef(0,0,1.5);
					glutSolidCone(0.2, 0.7, 16, 16);
				glPopMatrix();
			


				glBegin(GL_LINES);
				SelectedAxis == 4 ? glColor3f(1, 1, 0): glColor3f(0, 1, 0);
					glVertex3f(0, 1.5f, 0);
					glVertex3f(1.5f, 1.5f, 0);
				SelectedAxis == 4 ? glColor3f(1, 1, 0): glColor3f(1, 0, 0);
					glVertex3f(1.5f, 1.5f, 0);
					glVertex3f(1.5f, 0, 0);
					
				SelectedAxis == 5 ? glColor3f(1, 1, 0): glColor3f(0, 0, 1);
					glVertex3f(0, 0, 1.5f);
					glVertex3f(1.5f, 0, 1.5f);
				SelectedAxis == 5 ? glColor3f(1, 1, 0): glColor3f(1, 0, 0);
					glVertex3f(1.5f, 0, 1.5f);
					glVertex3f(1.5f, 0, 0);
					
				SelectedAxis == 6 ? glColor3f(1, 1, 0): glColor3f(0, 1, 0);
					glVertex3f(0, 1.5f, 0);
					glVertex3f(0, 1.5f, 1.5f);
				SelectedAxis == 6 ? glColor3f(1, 1, 0): glColor3f(0, 0, 1);
					glVertex3f(0, 1.5f, 1.5f);
					glVertex3f(0, 0, 1.5f);
				glEnd();
			glPopMatrix();

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
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
};