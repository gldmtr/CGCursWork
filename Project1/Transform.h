#pragma once
#include "Scene.h"
#include <gl\glut.h>

#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2

class Transform
{
public:
	Transform(void)
	{
		Mode = 0;
	};
	~Transform(void)
	{
	};
	void UpdateScene(int x, int y);
	void DrawTransformAxis();
	int GetSelectAxisObject(int x, int y);
	int Mode;
};

void Transform::UpdateScene(int x, int y)
{
	int dx = x - device->MouseX;
	int dy = y - device->MouseY;
	device->MouseX = x;
	device->MouseY = y;

	static Vector3f worldCursorPos = scene->GetPoint(x,y);

	scene->CursorPos = scene->GetPoint(x,y);
	Vector3f dimension = scene->CursorPos - worldCursorPos;

	SceneNode* node = scene->GetSelectedNode();
	if (node == NULL)   //поворот камеры
	{
		scene->CameraRotation += Vector3f((float)dy / (float)10, (float)dx / (float)10, 0);
		return;
	}
	if (!scene->SkipTransform)
	{
		switch (Mode)
		{
		case TRANSLATE:
#pragma region Translation
			if (scene->Translation.x())
			{
				node->Position.x(node->Position.x() + dimension.x());
			}
			if (scene->Translation.y())
			{
				node->Position.y(node->Position.y() + dimension.y());
			}
			if (scene->Translation.z())
			{
				node->Position.z(node->Position.z() + dimension.z());
			}
			break;
#pragma endregion
		case SCALE:
#pragma region Scaling
			if (scene->Translation.x())
			{
				node->Size.x(node->Size.x() + dimension.x());
			}
			if (scene->Translation.y())
			{
				node->Size.y(node->Size.y() + dimension.y());
			}
			if (scene->Translation.z())
			{
				node->Size.z(node->Size.z() + dimension.z());
			}
			break;
#pragma endregion
		case ROTATE:
#pragma region Rotation
			if (scene->Translation.x())
			{
				node->Angles.x(node->Angles.x() + dimension.x()*5);
			}
			if (scene->Translation.y())
			{
				node->Angles.y(node->Angles.y() + dimension.y()*5);
			}
			if (scene->Translation.z())
			{
				node->Angles.z(node->Angles.z() + dimension.z()*5);
			}
			break;
#pragma endregion
		default:
			break;
		}
	}
	worldCursorPos = scene->CursorPos;
	scene->SkipTransform = false;
}

void Transform::DrawTransformAxis()
{
	SceneNode *selected;
	int SelectedAxis = scene->SelectedAxis;
	if ((selected = scene->GetSelectedNode()) != NULL)
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

void PushAxis()
{
	glInitNames();
	glPushName(0);
	Vector3f pos = scene->GetSelectedNode()->Position;
	SceneNode* selected = scene->GetSelectedNode();
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

int Transform::GetSelectAxisObject( int x, int y )
{
	if (scene->GetSelectedNode() == NULL)
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

Transform* Transformation = new Transform();