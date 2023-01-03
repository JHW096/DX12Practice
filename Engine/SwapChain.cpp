#include "pch.h"
#include "SwapChain.h"


void SwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(info, dxgi, cmdQueue);
	CreateRTV(device);
}

void SwapChain::Present()
{
	//Present() ������ �̹����� ����ڿ��� �����ϴ� �Լ�
	_swapChain->Present(0, 0);
}

void SwapChain::Swapindex()
{
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CAHIN_BUFFER_COUNT;
	//0 -> 1 -> 0 -> 1
	//Swapchain�� �Ѵ�
}

void SwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	//�� �� ������ ������ ���� ��� ����
	_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(info.width);		//������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<uint32>(info.height);	//������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60;					//ȭ�� ���� ����
	sd.BufferDesc.RefreshRate.Denominator = 1;					//ȭ�� ���� ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// ������ ���÷��� ����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;									//��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SWAP_CAHIN_BUFFER_COUNT;					//���� + �ĸ� (2)
	sd.OutputWindow = info.hwnd;
	sd.Windowed = info.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;				//���� �ĸ� ���� ��ü�� ���� ������ ���� ����
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);

	for (int32 i = 0; i < SWAP_CAHIN_BUFFER_COUNT; i++)
	{
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
	}
}


void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	/*
		DescriptorHeap���� RenderTargetView����
		DX11�� RTV(RenderTargetView), DSV(DepthStencilView),
		CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)
	*/

	//RenderTargetView Size���
	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CAHIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CAHIN_BUFFER_COUNT; i++)
	{
		_rtvHaneld[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		//rendertarget�� 2�������� rendertargetview�� 2��
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHaneld[i]);
	}
}