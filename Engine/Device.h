#pragma once

class Device
{
private:

	ComPtr<ID3D12Debug> _debugController; //debug����
	ComPtr<IDXGIFactory> _dxgi;			//ȭ�����
	ComPtr<ID3D12Device> _device;		//���� ��ü ����

public:

	void Init();
	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDeivce() { return _device; }
};

