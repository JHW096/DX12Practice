#pragma once

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,		//�������� ���� �� ����.
	MESH_RENDERER,	//�������� ���� �� ����.
	//...
	MONO_BEHAVIOUR,	//������ ���� �� �ִ�.
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;


class Component
{
protected:

	COMPONENT_TYPE _type;
	weak_ptr<GameObject> _gameObject;

public:

	Component(COMPONENT_TYPE type);

	//�θ� Ŭ������ ���� ���̱� ������ �Ҹ��ڸ� virtual�� �ؾ� memory leak�� ������ �ʴ´�.
	virtual ~Component();

public:

	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() { }

public:

	COMPONENT_TYPE getType() { return _type; }
	bool isVaild() { return _gameObject.expired() == false; } //���ӿ�����Ʈ�� �Ҹ�Ǿ����� üũ

	shared_ptr<GameObject> getGameObject();
	shared_ptr<Transform> getTransform();

private:
	//�� ���� ������Ʈ������ ����ϵ���.
	friend class GameObject;
	void setGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }
};

