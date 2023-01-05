#pragma once
class ConstantBuffer
{
private:

	ComPtr<ID3D12Resource>	_cbvBuffer;					//GPU RAM Buffer
	BYTE*					_mappedBuffer{ nullptr };	//CPU DATA를 밀어넣을 때의 버퍼 
	uint32					_elementSize{ 0 };			//요소의 크기(Gpu ram buffer size)
	uint32					_elementCount{ 0 };			//요소의 갯수(GPU ram buffer count)

	uint32					_currentIndex{ 0 };			//어디까지 사용했는지 추척하기 위함
private:

	void CreateBuffer();													


public:

	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);
	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS getGpuVirtualAddress(uint32 index);

};

