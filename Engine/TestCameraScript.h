#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
private:

	float _speed = 100.0f;


public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

};