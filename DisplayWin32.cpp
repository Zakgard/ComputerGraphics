#include "DisplayWin32.h"

#include "Game.h"

Game* gameInstance;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		const auto key = static_cast<unsigned int>(wparam);
		//std::cout << "Key: " << key << std::endl;
		gameInstance->HandleInput(key);

		if (key == 27) PostQuitMessage(0);

		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

DisplayWin32::DisplayWin32(void* Data)
{
	g_Instance = GetModuleHandle(nullptr);

	gameInstance = reinterpret_cast<Game*>(Data);
	g_WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	g_WindowClass.lpfnWndProc = WndProc;
	g_WindowClass.cbClsExtra = 0;
	g_WindowClass.cbWndExtra = 0;
	g_WindowClass.hInstance = g_Instance;
	g_WindowClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	g_WindowClass.hIconSm = g_WindowClass.hIcon;
	g_WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	g_WindowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	g_WindowClass.lpszMenuName = nullptr;
	g_WindowClass.lpszClassName = DEFAULT_CLASS_NAME;
	g_WindowClass.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&g_WindowClass);

	g_ClientHeight = WINDOW_HEIGHT;
	g_ClientWidth = WINDOW_WIDTH;

	RECT windowRect = { 0, 0, static_cast<LONG>(g_ClientWidth), static_cast<LONG>(g_ClientHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	const auto posX = (GetSystemMetrics(SM_CXSCREEN) - g_ClientWidth) / 2;
	const auto posY = (GetSystemMetrics(SM_CYSCREEN) - g_ClientHeight) / 2;

	g_HWnd = CreateWindowEx(WS_EX_APPWINDOW, DEFAULT_CLASS_NAME, DEFAULT_CLASS_NAME,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		g_Instance,
		nullptr);

	ShowWindow(g_HWnd, SW_SHOW);
	SetForegroundWindow(g_HWnd);
	SetFocus(g_HWnd);

	ShowCursor(true);
}
