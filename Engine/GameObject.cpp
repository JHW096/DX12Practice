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
	//��� ������Ʈ�� Awake��
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
		���۷��� ī��Ʈ�� 0�� �� �� ū ���� �� �� ����.
		���� ����Ʈ �����͸� Ȱ���� this�� 
		����Ʈ �����Ͱ� this�� ���� �� �ֵ��� �ϴ� enable_shared_from_this<Ref>�� ��ӹ޾� ����Ѵ�.
	*/
	component->setGameObject(shared_from_this());

	//� Ÿ���� �߰��ϴ°�?
	uint8 index = static_cast<uint8>(component->getType());
	//MonoBehaviour�� �ƴ� �κ�
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	//��ũ��Ʈ(MonoBehaviour)
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}