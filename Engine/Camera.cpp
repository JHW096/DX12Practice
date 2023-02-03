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
	//invert(�����) ��� �ִ� ī�޶��� ���� ��Ŀ� ������� ���ָ� �亯ȯ���
	_matView = getTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(GEngine->getWindow().width);
	float height = static_cast<float>(GEngine->getWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE) //���� ����
	{
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	}
	else //Orthographic �������� ���� UI�� ���
	{
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);
	}

	_frustum.FinalUpdate();
}

void Camera::Render()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->getActiveScene();

	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameobject : gameObjects)
	{
		//MeshRenderer�� ������
		if (gameobject->GetMeshRenderer() == nullptr)
		{
			continue;
		}

		//Cull�� �������
		if (IsCulled(gameobject->GetLayerIndex()))
		{
			continue;
		}

		if (gameobject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameobject->GetTransform()->GetWorldPosition(),
				gameobject->GetTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}
		}

		gameobject->GetMeshRenderer()->Render();

	}

}
