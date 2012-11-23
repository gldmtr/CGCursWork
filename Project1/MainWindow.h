#pragma once
#include "Window.h"
#include <gl\glut.h>
#include <iostream>

class MainWindow : public Window
{
public: 
	MainWindow()
	{
		//Добавляем обработчики сообщений WM_CREATE и WM_DESTROY
		AddMessage(WM_CREATE,this,&MainWindow::OnCreate);
		AddMessage(WM_DESTROY,this,&MainWindow::OnDestroy);
		AddMessage(WM_PAINT,this,&MainWindow::OnPait);
		AddMessage(WM_SIZE,this,&MainWindow::OnResize);
	}

	LRESULT OnCreate(LPARAM lparam,WPARAM wparam)
	{
		return 0;
	}

	LRESULT OnDestroy(LPARAM lparam,WPARAM wparam)
	{
		PostQuitMessage(0);
		return 0;
	}

	LRESULT OnPait(LPARAM lparam,WPARAM wparam)
	{
		static float angle = 0;
		angle += 0.1f;
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		glColor3f(0,0,0);
		glTranslatef(0,0,-10);
		glRotatef(angle, 1,1,0);
		glutWireCube(1);

		SwapBuffers(hDC);
		glutPostRedisplay();
		return 0;
	}

	LRESULT OnResize(LPARAM lparam,WPARAM wparam)
	{
		GetWindowRect(hWnd, &windowRect);
		int width = windowRect.right - windowRect.left;
		int height = windowRect.bottom - windowRect.top;

		glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
		glLoadIdentity();                           // Reset The Projection Matrix
 
		// Calculate The Aspect Ratio Of The Window
		gluPerspective(90.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
		glViewport(0,0,width,height);
 
		glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
		glLoadIdentity();                           // Reset The Modelview Matrix

		return 0;
	}
};