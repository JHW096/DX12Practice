#pragma once

#include "Device.h"
#include "CommandQueue.h" 
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"

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
	shared_ptr<RootSignature> _rootSignature;
	shared_ptr<ConstantBuffer> _cb;
	shared_ptr<TableDescriptorHeap> _tableDescHeap;


public:

	shared_ptr<Device> getDevice()						{ return _device; }
	shared_ptr<CommandQueue> getCommandQueue()			{ return _cmdQueue; }
	shared_ptr<SwapChain> getSwapChain()				{ return _swapChain; }
	shared_ptr<RootSignature> getRootSignature()		{ return _rootSignature; }
	shared_ptr<ConstantBuffer> getCB()					{ return _cb;  }
	shared_ptr<TableDescriptorHeap> getTableDescHeap()	{ return _tableDescHeap;  }
public:

	void Init(const WindowInfo& window);
	void Render();

public:

	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

};

