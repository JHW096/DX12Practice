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
	//전체화면 모드 전환
	//디스플레이 모드 지원
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

	//CreateDevice
	// - Adaptor(그래픽 카드)객체
	// - nullptr = 기본 그래픽 카드
	// - mininu Featurelevel : 응용 프로그램(게임)이 요구하는 최소 기능 수준(구닥다리 제거)
	// - riid : 디바이스의 Com ID
	// - ppDevice : 생성된 장치가 매개변수에 설정 
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));

}
