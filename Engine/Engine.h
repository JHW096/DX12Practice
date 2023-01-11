#pragma once

#include "Device.h"
#include "CommandQueue.h" 
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"
#include "DepthStencilBuffer.h"

#include "Input.h"

class Engine
{
private:
	// 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport{ };
	D3D12_RECT		_scissorRect{ };

	//Render
	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<CommandQueue> _cmdQueue = make_shared<CommandQueue>();
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();
	shared_ptr<ConstantBuffer> _cb = make_shared<ConstantBuffer>();
	shared_ptr<TableDescriptorHeap> _tableDescHeap = make_shared<TableDescriptorHeap>();
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer = make_shared<DepthStencilBuffer>();
	
	//static Fn
	shared_ptr<Input> _input = make_shared<Input>();

public:

	shared_ptr<Device> getDevice()							{ return _device; }
	shared_ptr<CommandQueue> getCommandQueue()				{ return _cmdQueue; }
	shared_ptr<SwapChain> getSwapChain()					{ return _swapChain; }
	shared_ptr<RootSignature> getRootSignature()			{ return _rootSignature; }
	shared_ptr<ConstantBuffer> getCB()						{ return _cb;  }
	shared_ptr<TableDescriptorHeap> getTableDescHeap()		{ return _tableDescHeap;  }
	shared_ptr<DepthStencilBuffer> getDepthStencilBuffer()	{ return _depthStencilBuffer; }

	shared_ptr<Input> getInput()							{ return _input; }

public:

	void Init(const WindowInfo& window);
	void Render();

public:
	
	void Update();

public:

	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

};

