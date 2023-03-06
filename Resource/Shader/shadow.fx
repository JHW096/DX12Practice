#ifndef _SHADOW_FX_
#define _SHADOW_FX_

#include "params.fx"

struct VS_IN
{
	float3 pos : POSITION;
};

struct VS_OUT
{
	//System Position, PS로 진입시 pos는 pixel좌표로 변환 된다.
	float4 pos : SV_Position;	
	float4 clipPos : POSITION;
};

VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.0f;

	output.pos = mul(float4(input.pos, 1.0f), g_matWVP);
	output.clipPos = output.pos;

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
	return float4(input.clipPos.z / input.clipPos.w, 0.0f, 0.0f, 0.0f);
}

#endif