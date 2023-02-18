#include "pch.h"
#include "CommandQueue.h"
#include "Engine.h"

GraphicsCommandQueue::~GraphicsCommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void GraphicsCommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc{ };
	//Type_direct = GPU�� ���� ����
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//Flags_None = �⺻ ��� ť, others(FLAG_DISABLE_GPU_TIMEOUT) = GPUŸ�Ӿƿ� ��Ȱ��ȭ
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	//�̿ܿ��� desc�� Priority���� ����(���� �켱����(Normal)��, ���� �켱����(Height))���� ����
	//�̿ܿ��� desc�� NodeMask : �׷��� ī�带 ������ ������� �� ����. 0 = ���� GPU�� ���
	

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	_cmdList->Close(); //�ϰ��� �� �о�־���

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), NULL, IID_PPV_ARGS(&_resCmdList));

		
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

//���� ����
void GraphicsCommandQueue::WaitSync()
{
	_fenceValue++;

	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent); 

		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void GraphicsCommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	//queue clear�� ���� ���� 
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	int8 backIndex = _swapChain->GetBackBufferIndex();

	//
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_PRESENT,		//���� ���(before)
		D3D12_RESOURCE_STATE_RENDER_TARGET	//Backbuffer �۾�(after) 
		//�� swapchain�� ���ְڴ�. 
	);

	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	
	GEngine->getConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	GEngine->getConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();
	GEngine->getTableDescHeap()->Clear();
	ID3D12DescriptorHeap* descHeap = GEngine->getTableDescHeap()->getDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);

	//barrier�� list�� ����ִ´�. 
	_cmdList->ResourceBarrier(1, &barrier);

	//commmand list�� ������ reset�߱� ������ �ٽ� �۾��ؾ��Ѵٰ� �ϳ׿�.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);
}

void GraphicsCommandQueue::RenderEnd()
{
	int8 backIndex = _swapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		//RenderBegin()�� barrier���� �ݴ�
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);

	_cmdList->ResourceBarrier(1, &barrier); 
	_cmdList->Close();

	//Ŀ�ǵ� ����Ʈ ����
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present();

	//�� ��û�� �ذ�� ������ CPU�� ���
	WaitSync();

	_swapChain->Swapindex();

}

void GraphicsCommandQueue::FlushResourceCommandQueue()
{
	_resCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };

	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}
