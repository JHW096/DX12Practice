#include "pch.h"
#include "Device.h"

void Device::Init()
{
	////IID_PPV_ARGS(type) interface pointer return
#ifdef _DEBUG
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
#endif // _DEBUG

	//DirectX Graphics infrastructure
	//��üȭ�� ��� ��ȯ
	//���÷��� ��� ����
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

	//CreateDevice
	// - Adaptor(�׷��� ī��)��ü
	// - nullptr = �⺻ �׷��� ī��
	// - mininu Featurelevel : ���� ���α׷�(����)�� �䱸�ϴ� �ּ� ��� ����(���ڴٸ� ����)
	// - riid : ����̽��� Com ID
	// - ppDevice : ������ ��ġ�� �Ű������� ���� 
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));

}
