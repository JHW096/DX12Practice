#include "pch.h"
#include "Engine.h"



void Engine::Init(const WindowInfo& info)
{
	_window = info;
	ResizeWindow(info.width, info.height);
	
	//�׷��� ȭ�� ũ�⸦ ����
	//viewport(topleftX, topleftY, width, height, MinDepth, MaxDepth)
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	//typedef RECT D3D12_RECT;
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);


	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();

	_device->Init();
	_cmdQueue->Init(_device->getDeivce(), _swapChain);
	_swapChain->Init(info, _device->getDeivce(), _device->getDXGI(), _cmdQueue->getCmdQueue());
	_rootSignature->Init(_device->getDeivce());
}

void Engine::Render()
{
	RenderBegin();

	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{

	_window.width = width;
	_window.height = height;

	//::ǥ�� ���̺귯�� ����ߴٴ� �ǹ̷�
	RECT rect{ 0, 0, width, height };
	//AdjustwindowRect(Ŭ���̾�Ʈ ����, window style, menu(false))
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	//setwindowpos(� �������ΰ�, �������� ��ġ 0 = ���� ��, posX, posY, width, height, âũ�� ����)
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}
