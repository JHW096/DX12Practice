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
	//향후 Querternion으로 교체 예정
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	//물제크의 크기, 방향, 위치를 local에 삽입
	_matLocal = matScale * matRotation * matTranslation;
	//부모가 없으면 world = local;
	_matWorld = _matLocal;

	//부모 오브젝트가 있으면(parent != nullptr) 월드 좌표계 한 번 더 곱하기
	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();
	}
}

void Transform::PushData()
{
	//최종데이터를 GPU에 전송
	// [W]VP View는 카메라가 바라보는 좌표
	// TODO
	//CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_matWorld, sizeof(_matWorld));
	Matrix matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&matWVP, sizeof(matWVP));
}
