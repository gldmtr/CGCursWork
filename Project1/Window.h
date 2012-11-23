#pragma once
#include <Windows.h>
#include <map>

using namespace std;

class Window
{
public:
#pragma region Public_types
	//��� ��������� �� �������
	typedef LRESULT (Window::*FuncPointer)(LPARAM,WPARAM);

	//��������� ��������� �� �������-����������
	struct POINTER
	{
	Window* wnd;//��������� �� �����, �������� ����������� ����������
	FuncPointer func;
	};
#pragma endregion

#pragma region Public_fields
	HWND hWnd;//����� ������ ����
	RECT windowRect;
#pragma endregion

#pragma region Public_methods
	bool Create(
		HWND parent,//������������ ����, ���� 0 - �� ������� ����
		LPCSTR text,//��������� ����
		DWORD exstyle,DWORD style,//����� ����
		int x,int y,int w,int h,//������� � ���������
		UINT id//������������� ����
		)
	{
		//������������ ����� ����
		WNDCLASSEX wndc;
		wndc.lpszClassName=text;
		wndc.cbSize=sizeof(WNDCLASSEX);
		wndc.lpfnWndProc=WNDPROC(_WndProc);//������� ���������
		wndc.cbClsExtra=0;
		wndc.cbWndExtra=0;
		wndc.hbrBackground=HBRUSH(COLOR_WINDOW);//���� ���� ����
		wndc.hInstance=GetModuleHandle(0);//����� ����������
		wndc.hCursor=LoadCursor(0,IDC_ARROW);//��������� ������������ ������
		wndc.style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
		wndc.hIcon=0;
		wndc.hIconSm=0;
		wndc.lpszMenuName=0;
		RegisterClassEx(&wndc);

		//������� ���� ����
		hWnd=CreateWindowEx(exstyle,text,text,
			style|WS_CLIPCHILDREN,//����� WS_CLIPCHILDREN ����� ��� ����, ����� �������� �������� �� ������ ��� �����������
			x,y,w,h,parent,HMENU(id),
			GetModuleHandle(0),
			this//�������� � ������� ������� ��������� �� ����� ������ ����
			);

		if(!hWnd) return false;

		PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			16,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
	
		if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		{							// Reset The Display
			MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		{
			MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		{
			MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}
		PostMessage(hWnd, WM_SIZE, 2, NULL);
		return true;
	}
#pragma endregion

protected:
#pragma region Protected_fields
	map<UINT,POINTER> _msgmap;//����� ���������

	HDC hDC;		// Private GDI Device Context

	HGLRC hRC;		// Permanent Rendering Context

	unsigned int PixelFormat;
#pragma endregion

#pragma region Protected_methods
	template<typename T>
	bool AddMessage(UINT message,Window* wnd,LRESULT (T::*funcpointer)(LPARAM,WPARAM))
	{
		if(!wnd || !funcpointer) return false;

		POINTER msg;
		msg.wnd=wnd;
		msg.func=reinterpret_cast<FuncPointer>(funcpointer);

		_msgmap.insert(pair<UINT,POINTER>(message,msg));

		return true;
	}

	static LRESULT CALLBACK _WndProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam)
	{  
		Window *wnd=0;
		//��������� WM_NCCREATE �������� �� WM_CREATE
		//�.� ��� �������� ����
		if(message==WM_NCCREATE)
		{
			//�������� ��������� �� ��������� ������ ����, ������� �� �������� � ������� CreateWindowEx
			wnd=(Window*)LPCREATESTRUCT(lparam)->lpCreateParams;
			//� ��������� � ���� GWL_USERDATA
			SetWindowLong(hwnd,GWL_USERDATA,LONG(LPCREATESTRUCT(lparam)->lpCreateParams));
			wnd->hWnd=hwnd;      
		}
		//������ �������� ��������� �� ��� ��������� ����, �� ��� �� ���� GWL_USERDATA
		wnd=(Window*)GetWindowLong(hwnd,GWL_USERDATA);
		if(wnd)
		{
			//���� ��������� � �����
			map<UINT,POINTER>::iterator it;
			it=wnd->_msgmap.find(message);

			//���� ��������� �� �������, �� ������������ ��� �� ���������
			if(it==wnd->_msgmap.end()) 
			{
				return DefWindowProc(hwnd,message,wparam,lparam);
			}
			else
			{
				POINTER msg=it->second;        
				//�������� ������� ����������        
				LRESULT result=(msg.wnd->*msg.func)(lparam,wparam);
				if(result) return result;
			}
		}
		return DefWindowProc(hwnd,message,wparam,lparam);
	}
#pragma endregion


};