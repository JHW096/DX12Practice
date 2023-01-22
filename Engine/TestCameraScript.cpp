#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"

TestCameraScript::TestCameraScript()
{
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	Vec3 pos = getTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
	{
		pos += getTransform()->GetLook() * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::S))
	{
		pos -= getTransform()->GetLook() * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::A))
	{
		pos -= getTransform()->GetRight() * _speed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::D))
	{
		pos += getTransform()->GetRight() * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = getTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 0.5f;
		getTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = getTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 0.5f;
		getTransform()->SetLocalRotation(rotation);
	}

	getTransform()->SetLocalPosition(pos);
}
