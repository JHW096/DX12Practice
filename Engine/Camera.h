#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,	//원근
	ORTHOGRAPHIC	//직교	
};

class Camera : public Component
{
private:
//Projection m_var
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.0f;
	float _far = 1000.0f;
	float _fov = XM_PI / 4.0f; //field of view
	float _scale = 1.0f;

	Matrix _matView{ };
	Matrix _matProjection{ };

//Frustum m_var
	Frustum _frustum;

//Layer m_var
	uint32 _cullingMask = 0;

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

//Frustum Fn

	void SetProjectionType(PROJECTION_TYPE type) { _type = type; }

//Layer Fu
public:
	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
		{
			_cullingMask |= (1 << layer);
		}
		else
		{
			_cullingMask &= ~(1 << layer);
		}
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }
};

