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
	ComPtr<ID3D12Resource> _rtvBuffer[SWAP_CAHIN_BUFFER_COUNT]; //RenderTargets

	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	//uint32						_rtvHeapSize{ 0 }; _rtvHeap으로 구할 수 있으니 Pass
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHaneld[SWAP_CAHIN_BUFFER_COUNT];

	
	uint32 _backBufferIndex{ 0 };

private:

	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);

public:

	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

	void Present();
	void Swapindex();

	ComPtr<IDXGISwapChain> getSwapChain()
	{
		return _swapChain;
	}

	ComPtr<ID3D12Resource> getRenderTarget(int32 index)
	{
		return _rtvBuffer[index];
	}

	ComPtr<ID3D12Resource> getCurrentBackBufferResource()
	{
		return _rtvBuffer[_backBufferIndex];
	}
		
	D3D12_CPU_DESCRIPTOR_HANDLE getBackRTV() 
	{
		return _rtvHaneld[_backBufferIndex]; 
	}

};

