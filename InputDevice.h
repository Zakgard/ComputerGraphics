#pragma once

#include "Helpers.h"
#include "Game.h"

class InputDevice
{
public:
	InputDevice() noexcept;
	~InputDevice() noexcept;

	DirectX::XMFLOAT4 MouseOffset;
	DirectX::XMFLOAT4 MousePosition;

	bool MouseWheelDown;

	void AddPressedKey();
	bool IsKeyDown();
	void RemovePressed();

private:
	Game game;

	void OnKeyDown();
	void OnMouseMove();
};