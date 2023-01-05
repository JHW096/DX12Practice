#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
		{
			_cbvBuffer->Unmap(0, nullptr);
		}
		_cbvBuffer = nullptr;
	}
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//GPU Ram������ ������ ����
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer)
	);
	//cpu���� �о�ְ������ map���� ����
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	//���� unmap�� ���� �ʴ� ���� : �����͸� ��� ����ؼ� �����Ӹ��� �����ؼ� ����ϱ� ������
	//gpu�� �����͸� �о���� ���� ���۸� �ǵ�� �ȵǱ� ������, �� ����ȭ�� �ϹȾȴ뼭 
	//�̰� wait Sync����
}

void ConstantBuffer::Init(uint32 size, uint32 count)
{
	//��� ���۴� 256byte ����� ������ �������ִ�.
	// 0 256  512 ..
	//255(1111 1111) �� ���� and �� 256���� �ݳ��� �׷��� �츰 ��� ���۴� ���°� ���⿡ + 255�� ���ش�.
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

//rootParamIndex�� RootSignature���� ���� Constant register index�̴�.
void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	//assert�� ����� �ڵ�� �Ʒ��� ���� ������ �������� ������ Crash�Ѵ�.
	assert(_currentIndex < _elementSize);

	//�ش� index��ġ�� ��û�� buffer�� �����Ѵ�.
	//�� cpu�� �ִ� ������ gpu ram�� �ִ� buffer�� �����Ѵٴ� ��. /��� �Ͼ
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_GPU_VIRTUAL_ADDRESS address = getGpuVirtualAddress(_currentIndex);
	//gpu register�� �ϰ����
	//buffer�ּҸ� �����ؼ� register�� �־��ּ���. ��� �۾�
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	_currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::getGpuVirtualAddress(uint32 index)
{
	//gpubuffer�� �ּҸ� �޾ƿͼ� �� ��° �ּ����� �˷��ָ� return
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}


