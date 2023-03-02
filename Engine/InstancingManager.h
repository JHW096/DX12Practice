#pragma once
#include "InstancingBuffer.h"

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:

	void Render(vector<shared_ptr<GameObject>>& gameObjects);

	void ClearBuffer();
	void Clear() { _buffers.clear(); };

private:

	void AddParams(uint64 instanceId, InstancingParams& data);

private:
	uint64 _prevInstanceId = 0;
	map<uint64/*instanceId*/, shared_ptr<InstancingBuffer>> _buffers;
};

