#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"

#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"


void Engine::Init(const WindowInfo& info)
{
	_window = info;
	
	
	//그려질 화면 크기를 설정
	//viewport(topleftX, topleftY, width, height, MinDepth, MaxDepth)
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	//typedef RECT D3D12_RECT;
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device->Init();
	_cmdQueue->Init(_device->getDeivce(), _swapChain);
	_swapChain->Init(info, _device->getDeivce(), _device->getDXGI(), _cmdQueue->getCmdQueue());
	_rootSignature->Init();
	//_cb->Init(sizeof(Transform), 256); 단일 contantbuffer
	_tableDescHeap->Init(256);
	_depthStencilBuffer->Init(_window);

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 256);

	ResizeWindow(info.width, info.height);

	GET_SINGLE(Input)->Init(info.hwnd);
	GET_SINGLE(Timer)->Init();
	

}



void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();

	Render();

	ShowFPS();
}



void Engine::Render()
{
	RenderBegin();

	GET_SINGLE(SceneManager)->Render();

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

	//::표준 라이브러리 사용했다는 의미로
	RECT rect{ 0, 0, width, height };
	//AdjustwindowRect(클라이언트 영역, window style, menu(false))
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	//setwindowpos(어떤 윈도우인가, 윈도우의 위치 0 = 가장 앞, posX, posY, width, height, 창크기 조정)
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);

	_depthStencilBuffer->Init(_window);
}

void Engine::ShowFPS()
{
	uint32 fps = GET_SINGLE(Timer)->getFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_window.hwnd, text);

}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}
