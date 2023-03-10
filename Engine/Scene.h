#pragma once

class GameObject;

class Scene
{
private:

	vector<shared_ptr<GameObject>> _gameObjects;
	vector<shared_ptr<class Camera>> _cameras;
	vector<shared_ptr<class Light>> _lights;

public:

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();

	void ClearRTV();

	void RenderShadow();	/*Light이전에 호출해야한다.*/
	void RenderDeferred();
	void RenderLights();
	void RenderFinal();

	void RenderForward();
private:

	void PushLightData();


public:

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);
	
	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }
};

