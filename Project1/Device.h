#pragma once

#define NAV_UP		0
#define NAV_RIGHT	1
#define NAV_DOWN	2
#define NAV_LEFT	3

class Device
{
public:
	Device()
	{
		FovY = 60.0f;
		zNear = 0.1f;
		zFar = 200.0f;
		for (int i = 0; i< 4; i++)
		{
			NavigationKeys[i] = false;
		}
	};

	~Device()
	{
	};

	int Widht, Height;
	int MouseX, MouseY;
	float FovY, zNear, zFar;
	bool NavigationKeys[4];
};

static Device* device = new Device();