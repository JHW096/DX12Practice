#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

#include "GameObject.h"
#include "MeshRenderer.h"

#include "TestCameraScript.h"
#include "Resources.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
	{
		return;
	}

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

void SceneManager::Render()
{
	if (_activeScene)
	{
		_activeScene->Render();
	}
}

void SceneManager::LoadScene(wstring scenName)
{
	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
	{
		return 0;
	}

	return findIt->second;
}


shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");

#pragma endregion

	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->addComponent(make_shared<Transform>());
		camera->addComponent(make_shared<Camera>());		//near =1, far = 100, fov = 45
		camera->addComponent(make_shared<TestCameraScript>());
		//UI Culling
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
		//StartPosition
		camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		scene->AddGameObject(camera);
	}
	
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->addComponent(make_shared<Transform>());
		camera->addComponent(make_shared<Camera>());
		//Start Position
		camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		//직교 투영 카메라
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		//culling
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll();
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false);
		scene->AddGameObject(camera);
	}

#pragma endregion
	

//Prev to make gameobject
#pragma region SkyBox
	{
		shared_ptr<GameObject> skyBox = make_shared<GameObject>();
		skyBox->addComponent(make_shared<Transform>());
		skyBox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> skyBoxMeshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			skyBoxMeshRenderer->setMesh(sphereMesh);
		}
		{
			//high resolution space hdri
			shared_ptr<Shader> skyBoxShader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> skyBoxTexture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resource\\Texture\\Sky01.jpg");
			/*skyBoxShader->Init(
				L"..\\Resource\\Shader\\skybox.fx",
				{ RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL }
			);*/
			/*skyBoxTexture->Init(L"..\\Resource\\Texture\\Sky01.jpg");*/
			shared_ptr<Material> skyBoxMaterial = make_shared<Material>();
			skyBoxMaterial->setShader(skyBoxShader);
			skyBoxMaterial->setTexture(0, skyBoxTexture);
			//skyBoxMaterial->setTexture(1, skyBoxTexture);
			skyBoxMeshRenderer->setMaterial(skyBoxMaterial);
		}
		skyBox->addComponent(skyBoxMeshRenderer);
		scene->AddGameObject(skyBox);
	}
#pragma endregion

#pragma region Sphere

	shared_ptr<GameObject> sphere = make_shared<GameObject>();
	sphere->addComponent(make_shared<Transform>());
	sphere->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	sphere->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 150.0f));
	shared_ptr<MeshRenderer> sphereRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
		sphereRenderer->setMesh(sphereMesh);
	}
	{
		shared_ptr<Shader> sphereShader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
		shared_ptr<Texture> sphereTexture1 = 
			GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resource\\Texture\\Leather.jpg");
		shared_ptr<Texture> sphereTexture2 = 
			GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resource\\Texture\\Leather_Normal.jpg");
		shared_ptr<Material> sphereMaterial = make_shared<Material>();
		sphereMaterial->setShader(sphereShader);
		sphereMaterial->setTexture(0, sphereTexture1);
		sphereMaterial->setTexture(1, sphereTexture2);
		sphereRenderer->setMaterial(sphereMaterial);
	}
	sphere->addComponent(sphereRenderer);
	scene->AddGameObject(sphere);

#pragma endregion


//#pragma region Cube
//
//	shared_ptr<GameObject> Cube = make_shared<GameObject>();
//	Cube->addComponent(make_shared<Transform>());
//	Cube->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
//	Cube->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 150.0f));
//	shared_ptr<MeshRenderer> cubeRenderer = make_shared<MeshRenderer>();
//	{
//		shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
//		cubeRenderer->setMesh(cubeMesh);
//	}
//	{
//		shared_ptr<Shader> cubeShader = make_shared<Shader>();
//		shared_ptr<Texture> cubeTexture = make_shared<Texture>();
//		shared_ptr<Texture> cubeTexture2 = make_shared<Texture>();
//		cubeShader->Init(L"..\\Resource\\Shader\\forward.fx");
//		cubeTexture->Init(L"..\\Resource\\Texture\\Leather.jpg");
//		cubeTexture2->Init(L"..\\Resource\\Texture\\Leather_Normal.jpg");
//		shared_ptr<Material> cubeMaterial = make_shared<Material>();
//		cubeMaterial->setShader(cubeShader);
//		cubeMaterial->setTexture(0, cubeTexture);
//		cubeMaterial->setTexture(1, cubeTexture2);
//		cubeRenderer->setMaterial(cubeMaterial);
//	}
//	Cube->addComponent(cubeRenderer);
//	scene->AddGameObject(Cube);
//
//
//#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 5; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
		sphere->addComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		sphere->GetTransform()->SetLocalPosition(Vec3(-350.0f + (i * 160), 250.0f, 500.0f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->setMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
			{
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			}
			else
			{
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			}
			shared_ptr<Material> material = make_shared<Material>();
			material->setShader(shader);
			material->setTexture(0, texture);
			meshRenderer->setMaterial(material);
		}
		sphere->addComponent(meshRenderer);
		scene->AddGameObject(sphere);
	}
#pragma endregion

#pragma region Directional Light 
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->addComponent(make_shared<Transform>());
		light->addComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, 0, 1.0f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.0f, 0.0f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

		scene->AddGameObject(light);
	}

#pragma endregion

#pragma region  Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->addComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.0f, 150.0f));
		light->addComponent(make_shared<Light>());
		
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetLightRagne(200.0f);

		scene->AddGameObject(light);
	}

#pragma endregion 


#pragma region Spot Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->addComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(75.0f, 0.0f, 150.0f));
		light->addComponent(make_shared<Light>());

		light->GetLight()->SetLightDirection(Vec3(-1.0f, 0, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.0f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
		light->GetLight()->SetLightRagne(200.0f);
		light->GetLight()->SetLightAngle(3.14f / 2);

		scene->AddGameObject(light);

	}
#pragma endregion

	return scene;
}
