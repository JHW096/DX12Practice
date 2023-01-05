#include "pch.h"
#include "CommandQueue.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc{ };
	//Type_direct = GPU가 직접 실행
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//Flags_None = 기본 명령 큐, others(FLAG_DISABLE_GPU_TIMEOUT) = GPU타임아웃 비활성화
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	//이외에도 desc에 Priority선택 가능(보통 우선순위(Normal)과, 높은 우선순위(Height))조절 가능
	//이외에도 desc에 NodeMask : 그래픽 카드를 여러개 사용했을 때 설정. 0 = 단일 GPU일 경우
	

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	_cmdList->Close();
		
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

//향후 공부
void CommandQueue::WaitSync()
{
	_fenceValue++;

	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent); 

		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	//queue clear와 같은 개념 
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	//
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->getCurrentBackBufferResource().Get(),
		D3D12_RESOURCE_STATE_PRESENT,		//현재 출력(before)
		D3D12_RESOURCE_STATE_RENDER_TARGET	//Backbuffer 작업(after) 
		//즉 swapchain을 해주겠다. 
	);

	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	GEngine->getCB()->Clear();

	//barrier을 list에 집어넣는다. 
	_cmdList->ResourceBarrier(1, &barrier);

	//commmand list를 위에서 reset했기 때문에 다시 작업해야한다고 하네요.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	//어떤 버퍼에 그릴 것인가?
	//backbuffer를 알려줌
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->getBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr); //테스트겸 lightblue
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);


}

void CommandQueue::RenderEnd()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->getCurrentBackBufferResource().Get(),
		//RenderBegin()의 barrier과는 반대
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);

	_cmdList->ResourceBarrier(1, &barrier); 
	_cmdList->Close();

	//커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present();

	//위 요청이 해결될 때까지 CPU는 대기
	WaitSync();

	_swapChain->Swapindex();

}
