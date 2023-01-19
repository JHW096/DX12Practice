#pragma once
#include "Component.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,	//원근
	ORTHOGRAPHIC	//직교	
};

class Camera : public Component
{
private:

	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.0f;
	float _far = 1000.0f;
	float _fov = XM_PI / 4.0f; //field of view
	float _scale = 1.0f;

	Matrix _matView{ };
	Matrix _matProjection{ };

public:

	//TEMP
	static Matrix S_MatView;
	static Matrix S_MatProjection;

public:
	Camera();
	virtual ~Camera();

public:

	virtual void FinalUpdate() override;
	void Render();

};

