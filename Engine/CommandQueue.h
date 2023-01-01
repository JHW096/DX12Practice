#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
private:

	//CommandQueue : DX12에 등장
	//목록에 할 일을 저장했다가 한 방에 요청
	/*
	* ex) 
	* 주문 한개가 들어올 때 하나 씩 배송하는 것보단,
	* 오후 7시 까지 들어온 물량을 다음 날 한 번에 배송하는게 효율적
	*/
	ComPtr<ID3D12CommandQueue> _cmdQueue;		//
	ComPtr<ID3D12CommandAllocator> _cmdAlloc;	//일감 할당(helper)
	ComPtr<ID3D12GraphicsCommandList> _cmdList;	//일감 list

	//cpu, gpu 동기화 도구, CommandQueue와 같이 사용
	//모든 일처리가 끝날 때 까지 대기?..
	ComPtr<ID3D12Fence> _fence;
	uint32 _fenceValue{ 0 };
	HANDLE _fenceEvent{ INVALID_HANDLE_VALUE };

	shared_ptr<SwapChain> _swapChain;
	shared_ptr<DescriptorHeap> _descHeap;
 
public:

	void Init(
		ComPtr<ID3D12Device> device,
		shared_ptr<SwapChain> swapChain,
		shared_ptr<DescriptorHeap> descHeap
	);

	void WaitSync();

};

