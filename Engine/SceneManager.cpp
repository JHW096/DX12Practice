#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

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
	if (_activeScene == nullptr)
	{
		return;
	}

	const vector<shared_ptr<GameObject>>& gameObjects = _activeScene->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if(gameObject->getCamera() == nullptr)
			continue;

		gameObject->getCamera()->Render();
	}
}

void SceneManager::LoadScene(wstring scenName)
{
	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{

	shared_ptr<Scene> scene = make_shared<Scene>();


	//TEST Object
//#pragma region TestObject
//
//	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
//
//	vector<Vertex> vec(4);
//
//	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
//	vec[0].color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
//	vec[0].uv = Vec2(0.0f, 0.0f);
//
//	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
//	vec[1].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
//	vec[1].uv = Vec2(1.0f, 0.0f);
//
//	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
//	vec[2].color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
//	vec[2].uv = Vec2(1.0f, 1.0f);
//
//	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
//	vec[3].color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
//	vec[3].uv = Vec2(0.0f, 1.0f);
//
//	vector<uint32> indexVec;
//	{
//		indexVec.push_back(0);
//		indexVec.push_back(1);
//		indexVec.push_back(2);
//	}
//	{
//		indexVec.push_back(0);
//		indexVec.push_back(2);
//		indexVec.push_back(3);
//	}
//
//	gameObject->addComponent(make_shared<Transform>());
//	shared_ptr<Transform> transform = gameObject->getTransform();
//	transform->SetLocalPosition(Vec3(0.0f, 100.0f, 200.0f));
//	transform->SetLocalScale(Vec3(100.0f, 100.0f, 1.0f));
//
//
//	//mesh
//	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//	{
//		shared_ptr<Mesh> mesh = make_shared<Mesh>();
//		mesh->Init(vec, indexVec);
//		meshRenderer->setMesh(mesh);
//	}
//
//	//shader, texture
//	{
//		shared_ptr<Shader> shader = make_shared<Shader>();
//		shared_ptr<Texture> texture = make_shared<Texture>();
//
//		shader->Init(L"..\\Resource\\Shader\\default.hlsli");
//		texture->Init(L"..\\Resource\\Texture\\Gun.jpg");
//
//
//		shared_ptr<Material> material = make_shared<Material>();
//		material->setShader(shader);
//		material->setFloat(0, 0.3f);
//		material->setFloat(1, 0.4f);
//		material->setFloat(2, 0.3f);
//		material->setTexture(0, texture);
//		meshRenderer->setMaterial(material);;
//	}
//
//	gameObject->addComponent(meshRenderer);
//
//	scene->AddGameObject(gameObject);
//
//#pragma endregion

#pragma region Camera

	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->addComponent(make_shared<Transform>());
	camera->addComponent(make_shared<Camera>());		//near =1, far = 100, fov = 45
	camera->addComponent(make_shared<TestCameraScript>());

	camera->getTransform()->SetLocalPosition(Vec3(0.0f, 100.0f, 0.0f));
	scene->AddGameObject(camera);
	
#pragma endregion

#pragma region Sphere
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->addComponent(make_shared<Transform>());
		sphere->getTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		sphere->getTransform()->SetLocalPosition(Vec3(0.0f, 100.0f, 200.0f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->setMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"..\\Resource\\Shader\\default.hlsli");
			texture->Init(L"..\\Resource\\Texture\\Gun.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->setShader(shader);
			material->setTexture(0, texture);
			meshRenderer->setMaterial(material);
		}
		sphere->addComponent(meshRenderer);
		scene->AddGameObject(sphere);
	}

#pragma endregion

#pragma region Cube

	shared_ptr<GameObject> Cube = make_shared<GameObject>();
	Cube->addComponent(make_shared<Transform>());
	Cube->getTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	Cube->getTransform()->SetLocalPosition(Vec3(150.0f, 100.0f, 200.0f));
	shared_ptr<MeshRenderer> cubeRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
		cubeRenderer->setMesh(cubeMesh);
	}
	{
		shared_ptr<Shader> cubeShader = make_shared<Shader>();
		shared_ptr<Texture> cubeTexture = make_shared<Texture>();
		cubeShader->Init(L"..\\Resource\\Shader\\default.hlsli");
		cubeTexture->Init(L"..\\Resource\\Texture\\Gun.jpg");
		shared_ptr<Material> cubeMaterial = make_shared<Material>();
		cubeMaterial->setShader(cubeShader);
		cubeMaterial->setTexture(0, cubeTexture);
		cubeRenderer->setMaterial(cubeMaterial);
	}
	Cube->addComponent(cubeRenderer);
	scene->AddGameObject(Cube);


#pragma endregion

	return scene;
}
