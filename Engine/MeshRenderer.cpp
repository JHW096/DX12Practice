#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "InstancingBuffer.h"
#include "Resources.h"

MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	GetTransform()->PushData();
	_material->PushGraphicsData();
	_mesh->Render();
}

void MeshRenderer::Render(shared_ptr<class InstancingBuffer>& buffer)
{
	buffer->PushData();
	_material->PushGraphicsData();
	_mesh->Render(buffer);
}

void MeshRenderer::RenderShadow()
{
	GetTransform()->PushData();
	GET_SINGLE(Resources)->Get<Material>(L"Shadow")->PushGraphicsData();
	_mesh->Render();
}

//지금 당장은 Mesh와 Mateiral을 같은 것을 쓴다면, 동일 Object라고 알고 Instance id를 부여.
uint64 MeshRenderer::GetInstanceID()
{
	if (_mesh == nullptr || _material == nullptr)
	{
		return 0;
	}

	//uint64 id = (_mesh->GetID() << 32 | _material->GetID());
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}
