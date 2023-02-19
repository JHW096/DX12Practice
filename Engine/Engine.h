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
#include "RenderTargetGroup.h"

class Engine
{
private:
	// 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport{ };
	D3D12_RECT		_scissorRect{ };

	//Render
	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<GraphicsCommandQueue> _graphicsCmdQueue = make_shared<GraphicsCommandQueue>();
	shared_ptr<ComputeCommandQueue> _computeCmdQueue = make_shared<ComputeCommandQueue>();
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();
	shared_ptr<ConstantBuffer> _cb = make_shared<ConstantBuffer>();
	shared_ptr<GraphicsDescriptorHeap> _graphicsDescHeap = make_shared<GraphicsDescriptorHeap>();
	shared_ptr<ComputeDescriptorHeap> _computeDescHeap = make_shared<ComputeDescriptorHeap>();
	
	vector<shared_ptr<ConstantBuffer>> _constantBuffers;
	array<shared_ptr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> _rtGroups;
	//move static Fn
	/*shared_ptr<Input> _input = make_shared<Input>();
	shared_ptr<Timer> _timer = make_shared<Timer>();*/



public:

	const WindowInfo& GetWindow()								{ return _window; }

	shared_ptr<Device> GetDevice()								{ return _device; }
	shared_ptr<GraphicsCommandQueue> GetGraphicsCmdQueue()		{ return _graphicsCmdQueue; }
	shared_ptr<ComputeCommandQueue> GetComputeCmdQueue()		{ return _computeCmdQueue; }
	shared_ptr<SwapChain> GetSwapChain()						{ return _swapChain; }
	shared_ptr<RootSignature> GetRootSignature()				{ return _rootSignature; }
	//shared_ptr<ConstantBuffer> getCB()						{ return _cb;  }
	shared_ptr<GraphicsDescriptorHeap> GetGraphicsDescHeap()	{ return _graphicsDescHeap;  }
	shared_ptr<ComputeDescriptorHeap> GetComputeDescHeap()		{ return _computeDescHeap;  }

	//move static function
	/*shared_ptr<Input> getInput()							{ return _input; }
	shared_ptr<Timer> getTimer()							{ return _timer; }*/

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) 
	{
		return _constantBuffers[static_cast<uint8>(type)];
	}

	shared_ptr<RenderTargetGroup> GetRTGroup(RENDER_TARGET_GROUP_TYPE type)
	{
		return _rtGroups[static_cast<uint8>(type)];
	}

public:

	void Init(const WindowInfo& window);
	void Update();

public:

	//void LateUpdate();

public:

	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:

	void ShowFPS();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	void CreateRenderTargetGroups();

};


