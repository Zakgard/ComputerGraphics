#pragma once

#include "Helpers.h"
#include "DisplayWin32.h"
#include "ShaderManager.h"

class Game
{
public:
	Game() noexcept;
	virtual ~Game() noexcept;
	void HandleMsg() noexcept;
	void Exit() noexcept;
	void Run() noexcept;
	void FixedUpdate();
	void Update() noexcept;
	void RestoreTargets() noexcept;
	void DestroyResources() noexcept;
	void Draw() const noexcept;
	void EndFrame() noexcept;
	void Initialize(uint32_t mode) noexcept;
	void PrepareFrames() const noexcept;
	void PrepareResources(uint32_t objectsAmount) noexcept;
	void UpdateInternal() noexcept;
	void HandleInput(unsigned key) noexcept;

	ID3D11RenderTargetView* g_RenderTargetView;
	ComPtr<IDXGISwapChain> g_SwapChain;
	ComPtr<ID3D11Buffer> g_BackBuffer[3];
	ComPtr<ID3D11DeviceContext> g_Context;
	ComPtr<ID3D11Device> g_Device;
	const wchar_t* g_Name = L"Zakhar's game!";
	float g_DeltaTime;
	ComPtr<ID3D11RenderTargetView> g_RTV;
	float g_TotalTime;
	ID3D11RasterizerState* g_RastState;
	vector<ID3D11VertexShader*> g_VertexShaders;
	vector<ID3D11PixelShader*> g_PixelShaders;
	ID3DBlob* bc;
	ID3D11Texture2D* backTex = nullptr;
	DisplayWin32* g_Intance;

private:
	void CreateBackBuffer() noexcept;

	ID3D11InputLayout* g_Layout;
	D3D11_SUBRESOURCE_DATA g_Data;
	bool g_IsContinue;
	int g_FrameCount;

	uint16_t g_Mode;
};

