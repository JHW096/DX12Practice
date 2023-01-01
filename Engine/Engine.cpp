#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h" 
#include "SwapChain.h"
#include "DescriptorHeap.h"


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
	_descHeap = make_shared<ID3D12DescriptorHeap>();

	_device->Init();
}

void Engine::Render()
{
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