#pragma once

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,		//여러개를 가질 수 없다.
	MESH_RENDERER,	//여러개를 가질 수 없다.
	//...
	MONO_BEHAVIOUR,	//여러개 가질 수 있다.
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

	//부모 클래스로 사용될 것이기 때문에 소멸자를 virtual로 해야 memory leak이 생기지 않는다.
	virtual ~Component();

public:

	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() { }

public:

	COMPONENT_TYPE getType() { return _type; }
	bool isVaild() { return _gameObject.expired() == false; } //게임오브젝트가 소멸되었는지 체크

	shared_ptr<GameObject> getGameObject();
	shared_ptr<Transform> getTransform();

private:
	//꼭 게임 오브젝트에서만 사용하도록.
	friend class GameObject;
	void setGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }
};

