#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

void RootSignature::Init()
{
	CreateSamplerDesc();
	CreateRootSignature();
}

void RootSignature::CreateSamplerDesc()
{
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}

void RootSignature::CreateRootSignature()
{
	//rootDiscriptorTable type
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		//CBV_REGISTER_COUNT = 5, 0 b0~b4까지 총 5개 사용
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0), //b0부터 사용
		//texture를 위해 추가
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0) //b0부터 사용
	};


	CD3DX12_ROOT_PARAMETER param[1];
	//배열의 카운트와 범위
	param[0].InitAsDescriptorTable(_countof(ranges), ranges);

	//None Table ConstantBuffer
	//param[0].InitAsConstantBufferView(0); // b0 -> root CBV1
	//param[1].InitAsConstantBufferView(1); // b1 -> root CBV2


	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> bolobError;

	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &bolobError);
	DEVICE->CreateRootSignature(
		0,
		blobSignature->GetBufferPointer(),
		blobSignature->GetBufferSize(),
		IID_PPV_ARGS(&_signature)
	);
}


