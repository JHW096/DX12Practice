#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include"Engine.h"
#include"ConstantBuffer.h"
#include "Light.h"
#include "Engine.h"
#include "Resources.h"



void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FinalUpdate();
	}
}

void Scene::Render()
{
	PushLightData();

	//SwapChain Group 초기화
	uint8 backIndex = GEngine->getSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);

	//Deferred Group 초기화
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();

	//Lighting Group 초기화
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	//Deferred OMSet
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	shared_ptr<Camera> mainCamera = _cameras[0];
	mainCamera->SortGameObject();
	mainCamera->Render_Deferred();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	RenderLights();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();

	RenderFinal();

	mainCamera->Render_Forward();

	//메인 카메라 제외하고 나머지는 forward
	for (auto& Camera : _cameras)
	{
		if (Camera == mainCamera)
		{
			continue;
		}

		Camera->SortGameObject();
		Camera->Render_Forward();
	}

}

void Scene::RenderLights()
{
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	//광원을 그린다.
	for (auto& light : _lights)
	{
		light->Render();
	}
}

void Scene::RenderFinal()
{
	//Swapchain OMSet
	int8 backIndex = GEngine->getSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	GET_SINGLE(Resources)->Get<Material>(L"Final")->PushData();
	GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle")->Render();
}

void Scene::PushLightData()
{
	/*
	* 모든 오브젝트에서 lightCompnent를 가지는지 확인한다. 없으면 continue
	* 접근하여 lgihtinfo를 가져와서, 카운트를 증가시킨다.
	*/
	LightParams lightParams{ };

	for (auto& light : _lights)
	{
		const LightInfo& lightInfo = light->GetLightInfo();

		light->SetLightIndex(lightParams.lightCount);
		//TODO

		lightParams.lights[lightParams.lightCount] = lightInfo;
		lightParams.lightCount++;
	}
	//정보를 다 채운후 constbuffer를 이용해 정보입력
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGlobalData(&lightParams, sizeof(lightParams));

}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	//각 컴포넌트별로 나눔
	if (gameObject->GetCamera() != nullptr)
	{
		_cameras.push_back(gameObject->GetCamera());
	}
	else if(gameObject->GetLight() != nullptr)
	{
		_lights.push_back(gameObject->GetLight());
	}

	_gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{
	if (gameObject->GetCamera())
	{
		auto findIt = std::find(_cameras.begin(), _cameras.end(), gameObject->GetCamera());
		if (findIt != _cameras.end())
		{
			_cameras.erase(findIt);
		}
	}
	else if (gameObject->GetLight())
	{
		auto findIt = std::find(_lights.begin(), _lights.end(), gameObject->GetLight());
		if (findIt != _lights.end())
		{
			_lights.erase(findIt);
		}
	}

	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);
	if (findIt != _gameObjects.end())
	{
		_gameObjects.erase(findIt);
	}
}
