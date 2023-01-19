#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{
}

Transform::~Transform()
{
}

void Transform::FinalUpdate()
{
	Matrix matScale = Matrix::CreateScale(_localScale);
	//���� Querternion���� ��ü ����
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	//����ũ�� ũ��, ����, ��ġ�� local�� ����
	_matLocal = matScale * matRotation * matTranslation;
	//�θ� ������ world = local;
	_matWorld = _matLocal;

	//�θ� ������Ʈ�� ������(parent != nullptr) ���� ��ǥ�� �� �� �� ���ϱ�
	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();
	}
}

void Transform::PushData()
{
	//���������͸� GPU�� ����
	// [W]VP View�� ī�޶� �ٶ󺸴� ��ǥ
	// TODO
	//CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_matWorld, sizeof(_matWorld));
	Matrix matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&matWVP, sizeof(matWVP));
}
