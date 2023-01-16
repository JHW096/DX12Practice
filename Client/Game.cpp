#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"


//shared_ptr<Shader> shader = make_shared<Shader>();
//shared_ptr<Texture> texture = make_shared<Texture>();

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
	
	GET_SINGLE(SceneManager)->LoadScene(L"TestScene");

}


void Game::Update()
{
	GEngine->Update();

}