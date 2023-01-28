#pragma once
#include "Component.h"

enum class LIGHT_TYPE : uint8
{
	DIRECT_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightColor
{
	Vec4 diffuse;		//분산광
	Vec4 ambient;		//주변광
	Vec4 specular;		//반사광
};

struct LightInfo //->Shader
{
	LightColor color;		//빛의 색
	Vec4 position;			//발산 위치
	Vec4 direction;			//발사 방향
	int32 lightType;		//빛의 타입(enum class)
	float range;			//빛의 범위
	float angle;			//빛이 쏘는 각도
	int32 padding;			
};

struct LightParams			//shader에 넘겨줄 정보
{
	uint32 lightCount;		//조명의 갯수
	Vec3 padding;
	LightInfo lights[50];	//
};

class Light : public Component
{
private:

	LightInfo _lightInfo{ };

public:

	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;

public:

	const  LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(const Vec3& direction) { _lightInfo.direction = direction; }

	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { _lightInfo.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type) { _lightInfo.lightType = static_cast<uint32>(type); }
	void SetLightRagne(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

};

