#pragma once
#include "Component.h"

struct TransformMatrix
{
	Vec4 offset;
};

class Transform : public Component
{
private:

public:

	Transform();
	virtual ~Transform();

};

