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
		shared_ptr<Shader> sphereShader = make_shared<Shader>();
		shared_ptr<Texture> sphereTexture1 = make_shared<Texture>();
		shared_ptr<Texture> sphereTexture2 = make_shared<Texture>();
		sphereShader->Init(L"..\\Resource\\Shader\\forward.fx");
		sphereTexture1->Load(L"..\\Resource\\Texture\\Leather.jpg");
		sphereTexture2->Load(L"..\\Resource\\Texture\\Leather_Normal.jpg");
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
	{
		shared_ptr<GameObject> test = make_shared<GameObject>();
		test->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
		test->addComponent(make_shared<Transform>());
		test->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		test->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 500.0f));
		shared_ptr<MeshRenderer> testRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			testRenderer->setMesh(mesh);
		}
		{
			shared_ptr<Shader> testShader = GET_SINGLE(Resources)->Get<Shader>(L"Forward");
			shared_ptr<Texture> testTexture = GET_SINGLE(Resources)->Load<Texture>(L"Leather",
				L"..\\Resource\\Texture\\Leather.jpg");
			shared_ptr<Material> testMaterial = make_shared<Material>();
			testMaterial->setShader(testShader);
			testMaterial->setTexture(0, testTexture);
			testRenderer->setMaterial(testMaterial);
		}
		test->addComponent(testRenderer);
		scene->AddGameObject(test);
	}
#pragma endregion

#pragma region Green Directional Light 
	{
		shared_ptr<GameObject> directLight = make_shared<GameObject>();
		directLight->addComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.0f, 150.0f, 150.0f));

		directLight->addComponent(make_shared<Light>());
		directLight->GetLight()->SetLightDirection(Vec3(1.0f, 0.0f, 1.0f));
		directLight->GetLight()->SetLightType(LIGHT_TYPE::DIRECT_LIGHT);
		directLight->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		directLight->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		directLight->GetLight()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));

		scene->AddGameObject(directLight);
	}
#pragma endregion

//#pragma region Red Point Light
//	{
//		shared_ptr<GameObject> pointLight = make_shared<GameObject>();
//		pointLight->addComponent(make_shared<Transform>());
//		pointLight->GetTransform()->SetLocalPosition(Vec3(150.0f, 150.0f, 150.0f));
//		pointLight->addComponent(make_shared<Light>());
//		//pointLight->GetLight()->SetLightDirection(Vec3(0.0f, -1.0f, 0.0f));
//		pointLight->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//		pointLight->GetLight()->SetDiffuse(Vec3(1.0f, 0.1f, 0.1f));
//		pointLight->GetLight()->SetAmbient(Vec3(0.1f, 0.0f, 0.0f));
//		pointLight->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//		pointLight->GetLight()->SetLightRagne(10000.0f);
//		//pointLight->GetLight()->SetLightAngle(XM_PI / 4);
//		scene->AddGameObject(pointLight);
//	}
//
//#pragma endregion

//#pragma region Blue Spot Light
//	{
//		shared_ptr<GameObject> spotLight = make_shared<GameObject>();
//		spotLight->addComponent(make_shared<Transform>());
//		spotLight->GetTransform()->SetLocalPosition(Vec3(-150.0f, 0.0f, 150.0f));
//		spotLight->addComponent(make_shared<Light>());
//
//		spotLight->GetLight()->SetLightDirection(Vec3(1.0f, 0.0f, 0.0f));
//		spotLight->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		spotLight->GetLight()->SetDiffuse(Vec3(0.0f, 1.0f, 1.0f));
//		//spotLight->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
//		spotLight->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//		spotLight->GetLight()->SetLightRagne(10000.0f);
//		spotLight->GetLight()->SetLightAngle(XM_PI / 10);
//		scene->AddGameObject(spotLight);
//	}
//
//#pragma endregion

	return scene;
}
