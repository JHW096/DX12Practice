#pragma once

#include "Device.h"
#include "CommandQueue.h" 
#include "SwapChain.h"

class Engine
{
private:
	// 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport{ };
	D3D12_RECT		_scissorRect{ };

	shared_ptr<Device> _device;
	shared_ptr<CommandQueue> _cmdQueue;
	shared_ptr<SwapChain> _swapChain;

public:

	shared_ptr<Device> getDevice()				{ return _device; }
	shared_ptr<CommandQueue> getCommandQueue()	{ return _cmdQueue; }
	shared_ptr<SwapChain> getSwapChain()			{ return _swapChain; }

public:

	void Init(const WindowInfo& window);
	void Render();

public:

	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

};

