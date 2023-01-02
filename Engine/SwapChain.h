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
	ComPtr<ID3D12Resource> _renderTargets[SWAP_CAHIN_BUFFER_COUNT];
	uint32 _backBufferIndex{ 0 };

public:

	void Init(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

	void Present();
	void Swapindex();

	ComPtr<IDXGISwapChain> getSwapChain()
	{
		return _swapChain;
	}

	ComPtr<ID3D12Resource> getRenderTarget(int32 index)
	{
		return _renderTargets[index];
	}

	uint32 getCurrentBackBufferindex()
	{
		return _backBufferIndex;
	}

	ComPtr<ID3D12Resource> getCurrentBackBufferResource()
	{
		return _renderTargets[_backBufferIndex];
	}
		

};

