#pragma once
//한줄 요약 : 각종 리소스를 어떤 용도로 사용하는지 꼼꼼하게 적어서 넘겨주는 용도
//DX12에 새로 나온 것으로 DX11까지는 View라는 명칭

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

