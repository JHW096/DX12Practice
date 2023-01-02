#include "pch.h"
#include "Game.h"
#include "Engine.h"

void Game::Init(const WindowInfo& info)
{
	gp_Engine->Init(info);
	
}

void Game::Update()
{
	gp_Engine->Render();
}