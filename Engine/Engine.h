#pragma once
class Engine
{
private:
	// 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport{ };
	D3D12_RECT		_scissorRect{ };

	shared_ptr<class Device> _device;
	shared_ptr<class CommandQueue> _cmdQueue;
	shared_ptr<class SwapChain> _swapChain;
	shared_ptr<class DescriptorHeap> _descHeap;


public:

	void Init(const WindowInfo& window);
	void Render();

public:

	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

};

