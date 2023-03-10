#pragma once
#include "Object.h"

class Texture : public Object
{
private:
	//Resource
	ScratchImage					_image;
	D3D12_RESOURCE_DESC				_desc;
	ComPtr<ID3D12Resource>			_tex2D;

	//View
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;	
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	ComPtr<ID3D12DescriptorHeap>	_uavHeap;

private:
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHeapBegin = { };
	D3D12_CPU_DESCRIPTOR_HANDLE		_uavHeapBegin = { };


public:

	Texture();
	virtual ~Texture();

public:

	virtual void Load(const wstring& path) override;

public:

	//처음부터 다 만드는 case
	void Create(DXGI_FORMAT format, uint32 width, uint32 height,
		const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
		D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor = Vec4());

	//Resource가 있는 상태에서 textrue을 만드는 case
	//SwapChian같은 경우 : 내부 자체에 버퍼 리소스가 있고 Getbuffer를통해 가져옴.
	void CreateFromResource(ComPtr<ID3D12Resource> tex2D);

public:
	ComPtr<ID3D12Resource> GetTex2D() { return _tex2D; }
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return _srvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetRTV() { return _rtvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetDSV() { return _dsvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetUAV() { return _uavHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return _srvHeapBegin; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetUAVHandle() { return _uavHeapBegin; }

public:

	float GetWidth() { return static_cast<float>(_desc.Width); }
	float GetHeight() { return static_cast<float>(_desc.Height); }
};

