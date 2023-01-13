#pragma once

enum class CONSTANT_BUFFER_TYPE : uint8
{
	TRANSFORM,
	MATERIAL,
	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
};

class ConstantBuffer
{
private:

	ComPtr<ID3D12Resource>			_cbvBuffer;					//GPU RAM Buffer
	BYTE*							_mappedBuffer{ nullptr };	//CPU DATA를 밀어넣을 때의 버퍼 
	uint32							_elementSize{ 0 };			//요소의 크기(Gpu ram buffer size)
	uint32							_elementCount{ 0 };			//요소의 갯수(GPU ram buffer count)

	//DescriptorHeap
	ComPtr<ID3D12DescriptorHeap>	_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_cpuHandleBegin{ }; //시작 핸들 주소
	uint32							_handleIncrementSize{ 0 }; //몇 칸

	uint32							_currentIndex{ 0 };			//어디까지 사용했는지 추척하기 위함

	CBV_REGISTER					_reg{ };
private:

	void CreateBuffer();													
	void CreateView();

public:

	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);
	void Clear();

	void PushData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS getGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle(uint32 index);
};

