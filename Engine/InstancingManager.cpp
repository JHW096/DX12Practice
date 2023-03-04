#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	//<InstanceID, ���� InstanceID�� ���� Objects> cache
	map<uint64, vector<shared_ptr<GameObject>>> cache;

	//cache�� ��ü ��ġ
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		//��ü�� �� 1�����,
		if (vec.size() == 1)
		{
			//�Ϲ����� ����
			vec[0]->GetMeshRenderer()->Render();
		}
		//Instancing ����
		else
		{
			const uint64 instanceId = pair.first;

			//��� instacing ������ �ѱ�� �߰���.
			for (const shared_ptr<GameObject>& gameObject : vec)
			{
				InstancingParams params;
				params.matWorld = gameObject->GetTransform()->GetLocalToWorldMatrix();
				params.matWV = params.matWorld * Camera::S_MatView;
				params.matWVP = params.matWorld * Camera::S_MatView * Camera::S_MatProjection;

				AddParams(instanceId, params);
			}

			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetMeshRenderer()->Render(buffer);
		}
	}

}

void InstancingManager::ClearBuffer()
{
	for (auto& pair : _buffers)
	{
		shared_ptr<InstancingBuffer>& buffer = pair.second;
		buffer->Clear();
	}
}

void InstancingManager::AddParams(uint64 instanceId, InstancingParams& data)
{
	//find instance ID, ������ ����.
	if (_buffers.find(instanceId) == _buffers.end())
	{
		_buffers[instanceId] = make_shared<InstancingBuffer>();
	}

	_buffers[instanceId]->AddData(data);
}
