#pragma once
#include <Windows.h>

class App
{
public:
  void Run()
  {
	_done = false;
    MSG msg;
    while(!_done)
    {
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				_done=TRUE;							// If So done=TRUE
			}
			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
		}
		else
		{
			
		}
    }    
  }
private:
	bool _done;
};