#ifndef _LIGHTING_FX_
#define _LIGHTING_FX_

#include "params.fx"
#include "utils.fx"

struct VS_IN
{
	float3 pos : position;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

struct PS_OUT
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1;
};

//Direction Light Shader 
/*
* g_int_0 : Light index (Light Info Index)
* g_tex_0 : Position Render Target
* g_tex_1 : Normal Render Target
* Mesh : Rectangle (���� ǥ��)
*/
VS_OUT vs_DirLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	//���� ��ǥ��� -1.0 ~ 1.0 �̱� ������ Resources class�� width, height = 0.5�� ������ 2�� ����
	//�� ��üȭ���� ǥ��
	output.pos = float4(input.pos * 2.0f, 1.0f);
	output.uv = input.uv;

	return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	//Position �ش� ������ ��ü�� �ִ��� üũ
	float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
	//��ü�� ������, �� ī�޶� �ڿ� ������, ��ŵ
	if (viewPos.z <= 0.0f)
	{
		//clip(minus) = �ϰ͵� ����
		clip(-1);
	}

	//Normal
	float3 viewNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
	
	LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);
	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;

	return output;
}


/*
PointLight
g_int_0 : Light index
g_tex_0 : Position RenderTarget
g_tex_1 : Normal RenderTarget
g_vec2_0 : RenderTarget Resolution(�ػ� 800x600)
Mesh : Sphere ( ���� ������ ���� ���·� ǥ��)
*/

VS_OUT VS_PointLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), g_matWVP);
	output.uv = input.uv;

	return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	//input.pos = SV_Position = Screen ��ǥ
	//��������� uv�� input�� pixel��ǥ��� 800/600�ε�, uv��ǥ��(0~1)�� �ٲٱ� ���� ���� ���� ����
	float2 uv = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);
	//��ü üũ
	float3 viewPos = g_tex_0.Sample(g_sam_0, uv).xyz; 
	//������ ��ŵ
	if (viewPos.z <= 0.0f)
	{
		clip(-1);
	}

	int lightIndex = g_int_0;
	float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.0f), g_matView).xyz;
	//�ش� ��ü�� �������� ������ �Ÿ�
	float distance = length(viewPos - viewLightPos);
	//������ ��ŵ
	if (distance > g_light[lightIndex].range)
	{
		clip(-1);
	}

	//uv���� �̿��� Normal �� ����
	float3 viewNormal = g_tex_1.Sample(g_sam_0, uv).xyz;

	LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);

	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;

	return output;
}

#endif