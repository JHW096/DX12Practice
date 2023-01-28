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
	Vec4 diffuse;		//�л걤
	Vec4 ambient;		//�ֺ���
	Vec4 specular;		//�ݻ籤
};

struct LightInfo //->Shader
{
	LightColor color;		//���� ��
	Vec4 position;			//�߻� ��ġ
	Vec4 direction;			//�߻� ����
	int32 lightType;		//���� Ÿ��(enum class)
	float range;			//���� ����
	float angle;			//���� ��� ����
	int32 padding;			
};

struct LightParams			//shader�� �Ѱ��� ����
{
	uint32 lightCount;		//������ ����
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

