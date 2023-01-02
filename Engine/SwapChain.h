#pragma once
class SwapChain
{
private:

	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D12Resource> _renderTargets[SWAP_CAHIN_BUFFER_COUNT];
	uint32 _backBufferIndex{ 0 };

public:

	void Init(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
};

