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
* Mesh : Rectangle (영역 표시)
*/
VS_OUT vs_DirLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	//투영 좌표계는 -1.0 ~ 1.0 이기 때문에 Resources class의 width, height = 0.5씩 임으로 2를 곱함
	//즉 전체화면을 표시
	output.pos = float4(input.pos * 2.0f, 1.0f);
	output.uv = input.uv;

	return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	//Position 해당 지점에 물체가 있는지 체크
	float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
	//물체가 없으면, 즉 카메라 뒤에 있으면, 스킵
	if (viewPos.z <= 0.0f)
	{
		//clip(minus) = 암것도 안함
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
g_vec2_0 : RenderTarget Resolution(해상도 800x600)
Mesh : Sphere ( 빛의 영역을 구의 형태로 표시)
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

	//input.pos = SV_Position = Screen 좌표
	//결과적으로 uv는 input은 pixel좌표계로 800/600인데, uv좌표계(0~1)로 바꾸기 위해 각각 값을 나눔
	float2 uv = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);
	//물체 체크
	float3 viewPos = g_tex_0.Sample(g_sam_0, uv).xyz; 
	//없으면 스킵
	if (viewPos.z <= 0.0f)
	{
		clip(-1);
	}

	int lightIndex = g_int_0;
	float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.0f), g_matView).xyz;
	//해당 물체와 광원과의 사이의 거리
	float distance = length(viewPos - viewLightPos);
	//없으면 스킵
	if (distance > g_light[lightIndex].range)
	{
		clip(-1);
	}

	//uv값을 이용한 Normal 값 추출
	float3 viewNormal = g_tex_1.Sample(g_sam_0, uv).xyz;

	LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);

	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;

	return output;
}

#endif