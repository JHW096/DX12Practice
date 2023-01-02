#include "pch.h"
#include "DescriptorHeap.h"
#include "SwapChain.h"

void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;
	/*
		DescriptorHeap으로 RenderTargetView생성
		DX11의 RTV(RenderTargetView), DSV(DepthStencilView),
		CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)
	*/
	
	//RenderTargetView Size계산
	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CAHIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CAHIN_BUFFER_COUNT; i++)
	{
		_rtvHaneld[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		//rendertarget이 2개임으로 rendertargetview도 2개
		device->CreateRenderTargetView(swapChain->getRenderTarget(i).Get(), nullptr, _rtvHaneld[i]);
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::getBackBufferView()
{
	return getRTV(_swapChain->getCurrentBackBufferindex());
}
