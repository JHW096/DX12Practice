#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	//invert(역행렬) 찍고 있는 카메라의 월드 행렬에 역행렬을 해주면 뷰변환행렬
	_matView = getTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(GEngine->getWindow().width);
	float height = static_cast<float>(GEngine->getWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
	{
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	}
	else
	{
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);
	}

	S_MatView = _matView;
	S_MatProjection = _matProjection; 
}

void Camera::Render()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->getActiveScene();

	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameobject : gameObjects)
	{
		if (gameobject->GetMeshRenderer() == nullptr)
			continue;

		gameobject->GetMeshRenderer()->Render();

	}

}
