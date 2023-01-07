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

	//GPU Ram영역을 실제로 만듦
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer)
	);
	//cpu에서 밀어넣고싶으니 map으로 연결
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	//당장 unmap을 하지 않는 이유 : 데이터를 계속 사용해서 프레임마다 복사해서 사용하기 때문에
	//gpu가 데이터를 읽어들일 때는 버퍼를 건들면 안되기 때문에, 즉 동기화를 하믄안대서 
	//이건 wait Sync참고
}

void ConstantBuffer::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc{ };
	cbvDesc.NumDescriptors = _elementCount;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap));

	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < _elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = getCpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;
		cbvDesc.SizeInBytes = _elementSize; 

		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}

}

void ConstantBuffer::Init(uint32 size, uint32 count)
{
	//상수 버퍼는 256byte 배수로 만들라고 정해져있다.
	// 0 256  512 ..
	//255(1111 1111) 와 반전 and 즉 256으로 반내림 그러나 우린 상수 버퍼는 남는게 좋기에 + 255를 해준다.
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
	CreateView();
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

//rootParamIndex는 RootSignature에서 만든 Constant register index이다.
D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	//assert는 디버깅 코드로 아래와 같은 조건이 만족하지 않으면 Crash한다.
	assert(_currentIndex < _elementSize);

	//해당 index위치에 요청한 buffer를 복사한다.
	//즉 cpu에 있는 정보를 gpu ram에 있는 buffer에 복사한다는 것. /즉시 일어남
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = getCpuHandle(_currentIndex);

	//D3D12_GPU_VIRTUAL_ADDRESS address = getGpuVirtualAddress(_currentIndex);
	////gpu register에 일감등록
	////buffer주소를 참고해서 register에 넣어주세요. 라는 작업
	//CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	_currentIndex++;

	return cpuHandle;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::getGpuVirtualAddress(uint32 index)
{
	//gpubuffer의 주소를 받아와서 몇 번째 주소인지 알려주며 return
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::getCpuHandle(uint32 index)
{
	/*
		d3d12_cpu_descirptor_handle handle = _cpuHandleBegin;
		handle.ptr += index * _handleincreamentSize;
		return handle
	*/
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}


