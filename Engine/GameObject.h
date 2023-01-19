#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;

class GameObject : public enable_shared_from_this<GameObject>
	//enable_shared_from_this�� �� �ٸ� ���� �ƴ�,
	//weak_ptr<GameObject> _wptr�� ������ �Ͽ� shared_from_this�� ����� �� �ְ���.
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

