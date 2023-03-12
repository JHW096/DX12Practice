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
#include "ParticleSystem.h"

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

#pragma region ComputeShader 
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		//UAV용 Texture생성
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(
			L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM,
			1024,
			1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS //<-UAV
		);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->setShader(shader);
		material->setInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		//Thread Group( 1 * 1024 * 1 )
		material->Dispatch(1, 1024, 1);
	}
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

#pragma region Sphere / Object
	
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->addComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		obj->GetTransform()->SetLocalPosition(Vec3(0, 0.0f, 500.0f));
		obj->SetStatic(false);
		
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->setMesh(sphereMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
			meshRenderer->setMaterial(material->Clone());
		}
		obj->addComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
	
#pragma endregion

#pragma region Plane 
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->addComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(1000.0f, 1.0f, 1000.0f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.0f, -100.0f, 500.0f));
		obj->SetStatic(true);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->setMesh(mesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject")->Clone();
			material->setInt(0, 0);
			meshRenderer->setMaterial(material);
		}
		obj->addComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
		obj->addComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.0f + (i * 120), 250.0f, 500.0f));
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
			else if( i < 5)
			{
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			}
			else
			{
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
			}
			shared_ptr<Material> material = make_shared<Material>();
			material->setShader(shader);
			material->setTexture(0, texture);
			meshRenderer->setMaterial(material);
		}
		obj->addComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Directional Light 
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->addComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, 500));
		light->addComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}

#pragma endregion

//#pragma region  Point Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->addComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.0f, 150.0f));
//		light->addComponent(make_shared<Light>());
//		
//		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
//		light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
//		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
//		light->GetLight()->SetLightRagne(200.0f);
//
//		scene->AddGameObject(light);
//	}
//
//#pragma endregion 


//#pragma region Spot Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->addComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(75.0f, 0.0f, 150.0f));
//		light->addComponent(make_shared<Light>());
//
//		light->GetLight()->SetLightDirection(Vec3(-1.0f, 0, 0));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.0f, 0.5f));
//		light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
//		light->GetLight()->SetLightRagne(200.0f);
//		light->GetLight()->SetLightAngle(3.14f / 2);
//
//		scene->AddGameObject(light);
//
//	}
//#pragma endregion

//#pragma region ParticleSystem
//	{
//		shared_ptr<GameObject> particle = make_shared<GameObject>();
//		particle->addComponent(make_shared<Transform>());
//		particle->addComponent(make_shared<ParticleSystem>());
//		particle->SetCheckFrustum(false);
//		particle->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 100.0f));
//		scene->AddGameObject(particle);
//	}
//
//
//#pragma endregion


	return scene;
}
