#pragma once
class ConstantBuffer
{
private:

	ComPtr<ID3D12Resource>	_cbvBuffer;					//GPU RAM Buffer
	BYTE*					_mappedBuffer{ nullptr };	//CPU DATA�� �о���� ���� ���� 
	uint32					_elementSize{ 0 };			//����� ũ��(Gpu ram buffer size)
	uint32					_elementCount{ 0 };			//����� ����(GPU ram buffer count)

	//DescriptorHeap
	ComPtr<ID3D12DescriptorHeap>	_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_cpuHandleBegin{ }; //���� �ڵ� �ּ�
	uint32							_handleIncrementSize{ 0 }; //�� ĭ


	uint32					_currentIndex{ 0 };			//������ ����ߴ��� ��ô�ϱ� ����
private:

	void CreateBuffer();													
	void CreateView();

public:

	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);
	void Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS getGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle(uint32 index);
};
