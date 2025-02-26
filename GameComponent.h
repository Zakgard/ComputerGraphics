#pragma once

#include "Game.h"

class GameComponent
{
public:
	GameComponent() noexcept;
	GameComponent(const Game* game) noexcept;
	virtual ~GameComponent() noexcept;

	virtual void Draw() noexcept;
	void Initialize() noexcept;
	
	virtual void Reload() noexcept;
//	virtual void FixedUpdate(XMFLOAT4 offset) noexcept {};
	virtual void Update() noexcept;
	virtual void DestroyComponentDesources() noexcept;

	virtual GameComponent* GetGameComponent() noexcept;

	const Game* m_game;
};