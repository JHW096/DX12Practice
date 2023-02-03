#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;
class Light;

class GameObject : public Object, public enable_shared_from_this<GameObject>
	//enable_shared_from_this는 별 다른 것이 아닌,
	//weak_ptr<GameObject> _wptr를 가지게 하여 shared_from_this를 사용할 수 있게함.
{
private:

	//Component
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	//Script Component
	vector<shared_ptr<MonoBehaviour>> _scripts;

	//Frustum
	bool _checkFrustum{ true };
	//Layer
	uint8 _layerIndex = 0;

public:

	GameObject();
	virtual ~GameObject();


	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

//Component Fn
	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();
	
	void addComponent(shared_ptr<Component> component);

//Frustum Fn
	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }

//Layer Fn
	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }
};

