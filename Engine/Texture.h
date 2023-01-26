#pragma once
#include "Object.h"

class Texture : public Object
{
private:
	//Resource
	ScratchImage					_image;
	ComPtr<ID3D12Resource>			_tex2D;

	//View
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;	
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle = { };


public:

	Texture();
	virtual ~Texture();

public:

	void Init(const wstring& path);

	D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle() { return _srvHandle; }

public:

	void CreateTexture(const wstring& path);
	void CreateView();
};

