#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>
#include "App.h"
#include "MainWindow.h"
#include "Primitive.h"
#include "Scene.h"
#include "Device.h"
#include <iostream>
#include <math.h>

using namespace std;

#define Sin(x) sin(x/180*3.14)
#define Cos(x) cos(x/180*3.14)

void MotionFunc(int, int);

float angle = 0;

Scene *scene = new Scene();

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);                     
	glLoadIdentity();                    
 
	gluPerspective(device->FovY,(GLfloat)width/(GLfloat)(height+1),device->zNear,device->zFar);
 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();   

	device->Widht = width;
	device->Height = height;
}

void Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(scene->CameraRotation.x(),1,0,0);
	glRotatef(scene->CameraRotation.y(),0,1,0);
	glRotatef(scene->CameraRotation.z(),0,0,1);
	glTranslatef(-scene->CameraPosition.x(), -scene->CameraPosition.y(), -scene->CameraPosition.z());

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	for (int i = -50; i < 50; i++)
	{
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);

		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);
	}
	glEnd();

	for (int i = 0; i < scene->Nodes.size(); i++)
	{
		scene->Nodes[i]->Draw();
	}
	scene->DrawAxex();

	static int frame = 0, currenttime = 0, timebase = 0;
	char title[255] = {0};
	frame++;
 
	//get the current time
	currenttime = glutGet(GLUT_ELAPSED_TIME);
 
	//check if a second has passed
	if (currenttime - timebase > 1000) 
	{
		sprintf(title, "OpenGL CursWork v0.1   FPS: %4.2f",	frame*1000.0/(currenttime-timebase));
		glutSetWindowTitle(title);
	 	timebase = currenttime;		
		frame = 0;
	}

	glutSwapBuffers();

	glutPostRedisplay();
}

void MouseCallback(int button, int state, int x, int y)
{
	cout << button << "button pressed" << endl;
	if (state == GLUT_DOWN)
	{
		if (scene->IsNewObject)
		{
			scene->IsNewObject = false;
			scene->NewId = -1;
		}
		int selectedId = scene->GetSelectBufferObject(x,y);
		SceneNode* node = scene->GetObjectByID(selectedId);
		SceneNode* selectedNode = scene->GetSelectedNode();
		if (node == NULL)
		{
			scene->DisableSelection();
			return;
		}
		
		if ((selectedNode != NULL)&&(node->ID != selectedNode->ID))
		{
			selectedNode->Selected = false;
			node->Selected = true;
		}
		node->Selected = true;

		scene->CursorPos = scene->GetPoint(x,y);

		selectedNode = node;

		if ((selectedNode == NULL))
		{
			return;
		}

		int axis = scene->GetSelectAxisObject(x, y);
		switch(axis)
		{
		case 1:
			scene->Translation.x(true);
			break;
		case 2:
			scene->Translation.y(true);
			break;
		case 3:
			scene->Translation.z(true);
			break;
		case 4:
			scene->Translation.x(true);
			scene->Translation.y(true);
			break;
		case 5:
			scene->Translation.x(true);
			scene->Translation.z(true);
			break;
		case 6:
			scene->Translation.y(true);
			scene->Translation.z(true);
			break;
		}
		
		float winZ;
		glReadPixels(x, (float)device->Height - (float)y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		scene->zPoint = winZ;
	}
	if (state == GLUT_UP)
	{
		scene->Translation = Vector3<bool>(false, false, false);
	}
}

void MouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
		scene->CameraPosition += Vector3f(50 * Sin(scene->CameraRotation.y()), -50 * Sin(scene->CameraRotation.x()), -50 * Cos(scene->CameraRotation.y()));
    }
    else
    {
		scene->CameraPosition -= Vector3f(50 * Sin(scene->CameraRotation.y()), -50 * Sin(scene->CameraRotation.x()), -50 * Cos(scene->CameraRotation.y()));
    }

    return;
}

void PassiveMotionFunc(int x, int y)
{
	if (scene->IsNewObject)
	{
		scene->DisableSelection();
		scene->GetObjectByID(scene->NewId)->Selected = true;
		MotionFunc(x, y);
	}
	device->MouseX = x;
	device->MouseY = y;

	if (scene->GetSelectedNode() == NULL)
		return;
	scene->SelectedAxis = 0;
	scene->GetPoint(x, y).print();
	scene->SelectedAxis = scene->GetSelectAxisObject(x,y);
	
}

void MotionFunc(int x, int y)
{
	int dx = x - device->MouseX;
	int dy = y - device->MouseY;
	device->MouseX = x;
	device->MouseY = y;

	static Vector3f worldCursorPos = scene->GetPoint(x,y);

	scene->CursorPos = scene->GetPoint(x,y);

	SceneNode* node = scene->GetSelectedNode();
	if (node == NULL)   //поворот камеры
	{
		scene->CameraRotation += Vector3f((float)dy / (float)10, (float)dx / (float)10, 0);
		return;
	}
	else				//действия с объектом
	{
		GLdouble mat[16] = {0};
		glGetDoublev(GL_MODELVIEW_MATRIX, mat);

		float distance = Vector3f(node->Position - scene->CameraPosition).length();
		Vector3f dimension = scene->GetPoint(x,y) - worldCursorPos;
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
		node->Position.print();
	}
	worldCursorPos = scene->CursorPos;
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
		scene->Translation = Vector3<bool>(false, false, false);
		scene->Translation.x(true);
		break;
	case 'y':
		scene->Translation = Vector3<bool>(false, false, false);
		scene->Translation.y(true);
		break;
	case 'z':
		scene->Translation = Vector3<bool>(false, false, false);
		scene->Translation.z(true);
		break;
	case 'n':
		SceneNode* newNode = new Primitive(Vector3f(0,0,-10), 0, 1, Vector3f(0,0,0), false, 0);
		scene->AddNewNode(newNode);
		break;
	}
}

void TimerCallback(int delta)
{
	angle += 10.0f;

	if (device->NavigationKeys[NAV_UP])
		scene->CameraPosition += Vector3f(0.5 * Sin(scene->CameraRotation.y()), -0.5 * Sin(scene->CameraRotation.x()) , -0.5 * Cos(scene->CameraRotation.y()));

	if (device->NavigationKeys[NAV_DOWN])
		scene->CameraPosition -= Vector3f(0.5 * Sin(scene->CameraRotation.y()), -0.5 * Sin(scene->CameraRotation.x()) , -0.5 * Cos(scene->CameraRotation.y()));

	if (device->NavigationKeys[NAV_RIGHT])
		scene->CameraPosition += Vector3f(0.5 * Cos(scene->CameraRotation.y()), 0.5 * Sin(scene->CameraRotation.z()) , 0.5 * Sin(scene->CameraRotation.y()));

	if (device->NavigationKeys[NAV_LEFT])
		scene->CameraPosition -= Vector3f(0.5 * Cos(scene->CameraRotation.y()), 0.5 * Sin(scene->CameraRotation.z()) , 0.5 * Sin(scene->CameraRotation.y()));


	glutTimerFunc(50, TimerCallback, 50);
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		device->NavigationKeys[NAV_UP] = true;
	if (key == GLUT_KEY_DOWN)
		device->NavigationKeys[NAV_DOWN] = true;
	if (key == GLUT_KEY_RIGHT)
		device->NavigationKeys[NAV_RIGHT] = true;
	if (key == GLUT_KEY_LEFT)
		device->NavigationKeys[NAV_LEFT] = true;

}

void SpecialUpKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		device->NavigationKeys[NAV_UP] = false;
	if (key == GLUT_KEY_DOWN)
		device->NavigationKeys[NAV_DOWN] = false;
	if (key == GLUT_KEY_RIGHT)
		device->NavigationKeys[NAV_RIGHT] = false;
	if (key == GLUT_KEY_LEFT)
		device->NavigationKeys[NAV_LEFT] = false;
}

int main(int argc, char** argv)
{
	for ( int i = 0; i < 20; i++)
	{
		scene->Nodes.push_back(new Primitive(Vector3f(rand()%50 - 25, rand()%50, -rand()%25 - 5), 0, 3, 
			Vector3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX), false, 0));
	}
	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("MainWindow");
	glutDisplayFunc(Draw);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseCallback);
	glutMouseWheelFunc(MouseWheel);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutMotionFunc(MotionFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialKeyboard);
	glutSpecialUpFunc(SpecialUpKeyboard);
	glutTimerFunc(50, TimerCallback, 50);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(1,1,1,1);
	glutMainLoop();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
	return 0;
}