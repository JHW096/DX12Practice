#pragma once

class Device
{
private:

	ComPtr<ID3D12Debug> _debugController; //debug관련
	ComPtr<IDXGIFactory> _dxgi;			//화면관련
	ComPtr<ID3D12Device> _device;		//각종 객체 생성

public:

	void Init();
	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDeivce() { return _device; }
};

