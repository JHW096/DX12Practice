#pragma once
//���� ��� : ���� ���ҽ��� � �뵵�� ����ϴ��� �Ĳ��ϰ� ��� �Ѱ��ִ� �뵵
//DX12�� ���� ���� ������ DX11������ View��� ��Ī

class DescriptorHeap //View
{
private:
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	uint32							_rtvHeapSize{ 0 };
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHaneld[SWAP_CAHIN_BUFFER_COUNT];

	shared_ptr<class SwapChain>		_swapChain;

public:

	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE getRTV(int32 idx) { return _rtvHaneld[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE getBackBufferView();
};

