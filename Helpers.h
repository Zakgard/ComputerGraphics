#pragma once

#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>
#include <map>
#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOMENUS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI

#endif

#define NOMINMAX

#define STRICT
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <Windows.h>
#include <string>
#include <cstdint>
#include "DirectXCollision.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;

constexpr uint32_t WINDOW_HEIGHT{ 800 };
constexpr uint32_t WINDOW_WIDTH{ 800 };
constexpr uint16_t DEFAULT_LEFT_RECT{ 0 };
constexpr uint16_t DEFAULT_TOP_RECT{ 0 };

constexpr auto DEFAULT_CLASS_NAME = L"Zakhar's window class";
constexpr auto MAIN_WINDOW_NAME = L"Main Window";

constexpr auto DEFAULT_CURSOR_X_PLACEMENT{ 0 };
constexpr auto DEFAULT_CURSOR_Y_PLACEMENT{ 0 };
constexpr DirectX::XMFLOAT4 XMFLOAT4_BLOCK_VALUE{ -0.0f, 0.0f, -0.0f, 0.0f };

enum LABARATORY_MODE
{
	Architecture,
	Pong
};

static std::map<std::string, DirectX::XMFLOAT4> COLORS
{
	{ "White", { 1.0f, 1.0f, 1.0f, 1.0f} },
	{ "Black", { 0.0f, 0.0f, 0.0f, 1.0f} }
};

constexpr DirectX::XMFLOAT4 BALL_START_POS{ 0.1f, 0.1f , 0.1f , 0.1f };
constexpr int FRAMES_PER_SECOND = 60;
constexpr double UPDATE_DELAY = 1.0 / FRAMES_PER_SECOND;