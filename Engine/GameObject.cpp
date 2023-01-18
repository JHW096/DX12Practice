#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "MonoBehaviour.h"
#include "Component.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	addComponent(make_shared<Transform>());
}


void GameObject::Awake()
{
	//모든 컴포넌트를 Awake함
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
		{
			component->Awake();
		}
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
		{
			component->Start();
		}
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
		{
			component->Update();
		}
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
		{
			component->LateUpdate();
		}
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
		{
			component->FinalUpdate();
		}
	}

	
}

shared_ptr<Transform> GameObject::getTransform()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(_components[index]);
}



void GameObject::addComponent(shared_ptr<Component> component)
{
	/*
		component->setGameObject(make_shared<GameObejct>(this));
		레퍼런스 카운트가 0이 될 시 큰 일이 날 수 있음.
		따라서 스마트 포인터를 활용한 this는 
		스마트 포인터가 this를 가질 수 있도록 하는 enable_shared_from_this<Ref>를 상속받아 사용한다.
	*/
	component->setGameObject(shared_from_this());

	//어떤 타입을 추가하는가?
	uint8 index = static_cast<uint8>(component->getType());
	//MonoBehaviour가 아닌 부분
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	//스크립트(MonoBehaviour)
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}