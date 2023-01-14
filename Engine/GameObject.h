#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;

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

	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	shared_ptr<Transform> getTransform();

	void addComponent(shared_ptr<Component> component);
};

