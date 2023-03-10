#pragma once
#include "Component.h"

enum class LIGHT_TYPE : uint8
{
	DIRECTIONAL_LIGHT,
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

	int8 _lightIndex = -1;
	shared_ptr<class Mesh> _volumeMesh;
	shared_ptr<class Material> _lightMaterial;

	//After Shadow
	shared_ptr<GameObject> _shadowCamera;
public:

	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;
	void Render();
	void RenderShadow();

public:
	LIGHT_TYPE GetLightType() { return static_cast<LIGHT_TYPE>(_lightInfo.lightType); }

	const  LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(Vec3 direction);

	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { _lightInfo.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type);
	void SetLightRagne(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

	void SetLightIndex(uint8 index) { _lightIndex = index; }
};

