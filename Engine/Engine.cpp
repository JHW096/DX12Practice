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
	
	//그려질 화면 크기를 설정
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

	//::표준 라이브러리 사용했다는 의미로
	RECT rect{ 0, 0, width, height };
	//AdjustwindowRect(클라이언트 영역, window style, menu(false))
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	//setwindowpos(어떤 윈도우인가, 윈도우의 위치 0 = 가장 앞, posX, posY, width, height, 창크기 조정)
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}
