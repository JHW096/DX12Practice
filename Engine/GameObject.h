#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;

class GameObject : public enable_shared_from_this<GameObject>
	//enable_shared_from_this는 별 다른 것이 아닌,
	//weak_ptr<GameObject> _wptr를 가지게 하여 shared_from_this를 사용할 수 있게함.
{
private:

	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;

public:

	GameObject();
	virtual ~GameObject();


	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	shared_ptr<Transform> getTransform();
	shared_ptr<MeshRenderer> getMeshRenderer();
	shared_ptr<Camera> getCamera();

	void addComponent(shared_ptr<Component> component);
};

