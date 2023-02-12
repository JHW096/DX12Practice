#pragma once
/*
* SwapChain의 의미는 알고 있으니 패스.
*/

/*
	ID3DResource : renderTarget
*/
class SwapChain
{
private:

	ComPtr<IDXGISwapChain> _swapChain;
	uint32 _backBufferIndex{ 0 };

private:

	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

public:

	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

	void Present();
	void Swapindex();

	ComPtr<IDXGISwapChain> getSwapChain()
	{
		return _swapChain;
	}

	uint8 GetBackBufferIndex() { return _backBufferIndex; }

};

