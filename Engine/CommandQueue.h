#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
private:

	//CommandQueue : DX12�� ����
	//��Ͽ� �� ���� �����ߴٰ� �� �濡 ��û
	/*
	* ex) 
	* �ֹ� �Ѱ��� ���� �� �ϳ� �� ����ϴ� �ͺ���,
	* ���� 7�� ���� ���� ������ ���� �� �� ���� ����ϴ°� ȿ����
	*/
	ComPtr<ID3D12CommandQueue> _cmdQueue;		//
	ComPtr<ID3D12CommandAllocator> _cmdAlloc;	//�ϰ� �Ҵ�(helper)
	ComPtr<ID3D12GraphicsCommandList> _cmdList;	//�ϰ� list

	//cpu, gpu ����ȭ ����, CommandQueue�� ���� ���
	//��� ��ó���� ���� �� ���� ���?..
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

