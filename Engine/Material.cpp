#include "pch.h"
#include "Material.h"
#include "Engine.h"

void Material::Update()
{
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
		{
			continue;
		}

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->getTableDescHeap()->setSRV(_textures[i]->getCpuHandle(), reg);
	}

	_shader->Update();
}
