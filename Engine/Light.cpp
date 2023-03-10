#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Resources.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "Texture.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Engine.h"

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
	//After Shadow
	//실실적으로 찍는 것이 아닌(Scene에 존재하지 않는다.), Light Compoenent가 그림자를 찍기 위한 내부적으로 존재
	_shadowCamera = make_shared<GameObject>();
	_shadowCamera->addComponent(make_shared<Transform>());
	_shadowCamera->addComponent(make_shared<Camera>());
	uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
	_shadowCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	_lightInfo.position = GetTransform()->GetWorldPosition();
	
	//After Shadow
	//Camera의 좌표도 Light와 똑같이 맞추어줌.
	_shadowCamera->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	_shadowCamera->GetTransform()->SetLocalRotation(GetTransform()->GetLocalRotation());
	_shadowCamera->GetTransform()->SetLocalScale(GetTransform()->GetLocalScale());

	_shadowCamera->FinalUpdate();
}

void Light::Render()
{
	assert(_lightIndex >= 0);

	GetTransform()->PushData(); 

	if (static_cast<LIGHT_TYPE>(_lightInfo.lightType) == LIGHT_TYPE::DIRECTIONAL_LIGHT)
	{
		shared_ptr<Texture> shadowTex = GET_SINGLE(Resources)->Get<Texture>(L"ShadowTarget");
		_lightMaterial->setTexture(2, shadowTex);

		Matrix matVP = _shadowCamera->GetCamera()->GetViewMatrix() * _shadowCamera->GetCamera()->GetProjectionMatrix();
		_lightMaterial->SetMatrix(0, matVP);
	}
	else
	{
		float scale = 2 * _lightInfo.range;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
	}

	_lightMaterial->setInt(0, _lightIndex);
	_lightMaterial->PushGraphicsData();

	_volumeMesh->Render();
}

void Light::RenderShadow()
{
	//TODO
	_shadowCamera->GetCamera()->SortShadowObject();
	_shadowCamera->GetCamera()->Render_Shadow();
}

void Light::SetLightDirection(Vec3 direction)
{
	direction.Normalize();

	_lightInfo.direction = direction;

	GetTransform()->LookAt(direction);
}

void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo.lightType = static_cast<int32>(type);

	switch (type)
	{
		case LIGHT_TYPE::DIRECTIONAL_LIGHT:
			_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle");
			_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");

			_shadowCamera->GetCamera()->SetScale(1.0f);
			_shadowCamera->GetCamera()->SetFar(10000.0f);
			_shadowCamera->GetCamera()->SetWidth(4096);
			_shadowCamera->GetCamera()->SetHeight(4096);

			break;
		case LIGHT_TYPE::POINT_LIGHT:
			_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
			_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
			break;
		case LIGHT_TYPE::SPOT_LIGHT:
			_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
			_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
			break;
	}
}
