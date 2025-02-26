#pragma once

#include "Helpers.h"

class DisplayWin32
{
public:
	DisplayWin32(void* Data);
	virtual ~DisplayWin32() {};

	uint32_t g_ClientHeight;
	uint32_t g_ClientWidth;
	HINSTANCE g_Instance;
	HWND g_HWnd;
	WNDCLASSEX g_WindowClass;

	static DisplayWin32 *Instance;
};

