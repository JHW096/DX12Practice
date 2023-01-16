#pragma once

class GameObject;

class Scene
{
private:

	vector<shared_ptr<GameObject>> _gameObjects;

public:

	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);
	
};

