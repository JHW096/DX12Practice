#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	DEFFERED,
	FORWARD,
	LIGHTING,
};

enum class RASTERIZER_TYPE : uint8
{
	CULL_NONE,		//모두 다 연산
	CULL_FRONT,		//시계 무시
	CULL_BACK,		//기본 상태, 반시계 무시,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE : uint8
{
	//깊이 값이 다르더라도 연산하도록
	LESS,				
	LESS_EQUAL,			
	GREATER,
	GREATER_EQUAL,
	NO_DEPTH_TEST,			//깊이 테스트x, 깊이 기록 o
	NO_DEPTH_TEST_NO_WRITE,	//깊이 테스트x, 깊이 기록 o
	LESS_NO_WRITE,			//깊이 테스트o, 깊이 기록 x
};

enum class BLEND_TYPE : uint8
{
	DEFAULT,
	ALPHA_BLEND,		//투명도를 이용한 블랜드
	ONE_TO_ONE_BLEND,
	END,
};

struct ShaderInfo
{
	//기본 설정
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;

	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};


class Shader : public Object
{
private:
	ShaderInfo _info;

	ComPtr<ID3DBlob> _vsBlob;
	ComPtr<ID3DBlob> _psBlob;
	ComPtr<ID3DBlob> _errBlob;

public:
	
	Shader();
	virtual ~Shader();

public:

	void Init(
		const wstring& path,
		ShaderInfo info = ShaderInfo(),
		const string& vs = "VS_Main",
		const string& ps = "PS_Main"
		);

	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

	ComPtr<ID3D12PipelineState> _pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _pipelineDesc{ };

};

