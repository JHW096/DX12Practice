#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
shared_ptr<Texture> texture = make_shared<Texture>();

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
		
	vector<Vertex> vec(4);
	
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec[0].uv = Vec2(0.0f, 0.0f);

	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec[1].uv = Vec2(1.0f, 0.0f);

	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec[2].uv = Vec2(1.0f, 1.0f);

	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec[3].uv = Vec2(0.0f, 1.0f);
	
	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	mesh->Init(vec, indexVec);
	
	shader->Init(L"..\\Resource\\shader\\default.hlsli");

	texture->Init(L"..\\Resource\\Texture\\Gun.jpg");
	
	GEngine->getCommandQueue()->WaitSync();
}

//only VertexBuffer used
//void Game::Init(const WindowInfo& info)
//{
//	GEngine->Init(info);
//	
//	vector<Vertex> vec(6);
//
//	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
//	vec[0].color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
//
//	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
//	vec[1].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
//
//	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
//	vec[2].color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
//
//	vec[3].pos = Vec3(0.5f, -0.5f, 0.5f);
//	vec[3].color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
//
//	vec[4].pos = Vec3(-0.5f, -0.5f, 0.5f);
//	vec[4].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
//
//	vec[5].pos = Vec3(-0.5f, 0.5f, 0.5f);
//	vec[5].color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
//
//	mesh->Init(vec);
//
//	shader->Init(L"..\\Resource\\shader\\default.hlsli");
//
//	GEngine->getCommandQueue()->WaitSync();
//
//}

void Game::Update()
{
	GEngine->Update();

	GEngine->RenderBegin();

	shader->Update();
	{
		static Transform t{ };
		
		if (INPUT->GetButton(KEY_TYPE::W))
		{
			t.offset.y += 1.0f * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::S))
		{
			t.offset.y -= 1.0f * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::A))
		{
			t.offset.x -= 1.0f * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::D))
		{
			t.offset.x += 1.0f * DELTA_TIME;
		}

		mesh->setTransform(t);

		mesh->setTexture(texture);

		mesh->Render();
	}



	/*{
		Transform t;
		t.offset = Vec4(0.0f, 0.0f, 0.3f, 0.0f);
		mesh->setTransform(t);

		mesh->setTexture(texture);

		mesh->Render();
	}*/

	


	GEngine->RenderEnd();
}