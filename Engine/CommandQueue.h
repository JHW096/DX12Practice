#pragma once
#include "SwapChain.h"
class SwapChain;
class DescriptorHeap;

//*******************************
//GraphicsCommandQueue
//*******************************

/*
	ID3DCommandQueue�� DX12�� ���� ������
	GPU���� ��� ��⿭(Command Queue)�� �ִ�.
	CPU�� ��� ���(Command List)�� DirectX3D API�� ���� ��⿭�� �����Ѵ�.
	GPU�� ����� �ٷ� �޴� ���� �ƴ�, ������ ����� ��ɵ��� ó���� ���Ŀ� �޴´�.
	��, ��� ��⿭�� ���GPU�� ������ �Ұ�, ��⿭�� ������ CPU�� ��Եȴ�.
	���ӿ����� �̷��� ��Ȳ�� �ٶ������� �ʰԺ���.
	���� ��� : CommandQueue�� ExcuteCommandList()�� ���� GPU�� Command(���)�Ѵ�.
	���� ��� : CreateCommandQueue()
	�ʼ� ���� : D3D12_Command_Queue_Desc
*/

/*
	ID3D12CommanAllocator
	CommandQueue ���ο� �ִ� Command List���� �Ҵ��Ѵ�.
	Allocator�� List�� ������ 1:1
*/

/*
	ID3DCommandList
	List�� �Լ����� Setter, Draw(�׸��� ȣ��)
	List�� ����� ���ؼ��� Allocator �������̽� ������ �ʿ�
	Open/Close�� �̿��� ��� �߰�, �߰� �Ұ� ���� ���� ��� ����
*/


class GraphicsCommandQueue
{
private:

	ComPtr<ID3D12CommandQueue> _cmdQueue;		//
	ComPtr<ID3D12CommandAllocator> _cmdAlloc;	//�ϰ� �Ҵ�(helper)
	ComPtr<ID3D12GraphicsCommandList> _cmdList;	//�ϰ� list


	//-------------resource Node��
	ComPtr<ID3D12CommandAllocator> _resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> _resCmdList;

	//cpu, gpu ����ȭ ����, CommandQueue�� ���� ���
	//��� ��ó���� ���� �� ���� ���?..
	ComPtr<ID3D12Fence> _fence;
	uint32 _fenceValue{ 0 };
	HANDLE _fenceEvent{ INVALID_HANDLE_VALUE };

	shared_ptr<SwapChain> _swapChain;
 
public:

	~GraphicsCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCmdList() { return _cmdList; }

	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return _resCmdList; }

public:

	void Init(
		ComPtr<ID3D12Device> device,
		shared_ptr<SwapChain> swapChain
	);



	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();


	void FlushResourceCommandQueue();
};

//*******************************
//Compute CommandQueue
//*******************************

class ComputeCommandQueue
{
public:
	~ComputeCommandQueue();

public:

	void Init(ComPtr<ID3D12Device> device);
	void WaitSync();
	void FlushComputeCommandQueue();

public:

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetComputeCmdList() { return _cmdList; }

private:

	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;
};