#pragma once

#include "Helpers.h"

DirectX::XMFLOAT4 SQUARE_POINTS[4] = {
        DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 0.0f),	DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 0.0f),
        DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 0.0f),      DirectX::XMFLOAT4(-0.5f, 0.5f, 0.0f, 0.0f),
};

DirectX::XMFLOAT4 DOT_POINTS[4] = {
        DirectX::XMFLOAT4(-0.05f, 0.97f, 0.0f, 0.0f),	 DirectX::XMFLOAT4(0.0f, 0.97f, 0.0f, 0.0f),
        DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),      DirectX::XMFLOAT4(-0.05f, 1.0f, 0.0f, 0.0f),
};

constexpr uint16_t QUADS_IN_GRID = 29;
constexpr float QUADS_IN_GRID_POS_OFFSET = -0.07f;
constexpr float PLAYER_POS_OFFSET = 1.5f;

DirectX::XMFLOAT4 PLAYERS_START_POINTS[4] = {
        DirectX::XMFLOAT4(-0.95f, 0.6f, 0.0f, 0.0f),	    DirectX::XMFLOAT4(-0.85f, 0.6f, 0.0f, 0.0f),
        DirectX::XMFLOAT4(-0.85f, 0.8f, 0.0f, 0.0f),     DirectX::XMFLOAT4(-0.95f, 0.8f, 0.0f, 0.0f),
};

constexpr float PLAYER_MOVE_SPEED = 0.1f;