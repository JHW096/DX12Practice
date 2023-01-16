#pragma once

class Scene;

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

private:
	shared_ptr<Scene> _activeScene;

public:

	void Update();

	void LoadScene(wstring scenName);

public:

	shared_ptr<Scene> getActiveScene() { return _activeScene; }

//TEST CORD
private:
	shared_ptr<Scene> LoadTestScene();

};

