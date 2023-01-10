#pragma once

class DepthStencilBuffer
{
private:
	//DepthStencilView
	ComPtr<ID3D12Resource> _dsvBuffer;
	ComPtr<ID3D12DescriptorHeap> _dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _dsvHandle{ };
	DXGI_FORMAT _dsvFormat{ };

public:
	//Depth32용도로만 우선 사용 
	void Init(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

	D3D12_CPU_DESCRIPTOR_HANDLE getDSVCpuHandle() { return _dsvHandle; }
	DXGI_FORMAT getDSVFormat() { return _dsvFormat; }

};

