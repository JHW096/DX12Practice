#pragma once

class Scene;

enum
{
	//0~31
	MAX_LAYER = 32
};


class SceneManager
{
	DECLARE_SINGLE(SceneManager);

private:
	shared_ptr<Scene> _activeScene;

//Layer
private:

	array<wstring, MAX_LAYER> _layerNames;
	//layer의 index번호를 찾기 위해
	map<wstring, uint8> _layerIndex;

public:

	void Update();
	void Render();
	void LoadScene(wstring scenName);

//layer Fn
public:

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:

	shared_ptr<Scene> getActiveScene() { return _activeScene; }

//TEST CORD
private:
	shared_ptr<Scene> LoadTestScene();

};

