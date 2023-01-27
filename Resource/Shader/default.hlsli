
cbuffer TRANSFORM_PARAMS : register(b0)
{
    //directX행렬 순서, 셰이더 접근 순서가 다르기 때문에 DX규칙으로 만듦.
    /*
       DX접근 방법 : 00, 01, 02 ..
       Shader 방법 : 00, 10, 20 ..
       따라서 row_major는 DX접근 방식인 행이동 부터 접근하도록 하라는 뜻.
    */
    row_major matrix matWVP;
};

//material
cbuffer MATERIAL_PARAMS : register(b1)
{
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    int int_4;

    float float_0;
    float float_1;
    float float_2;
    float float_3;
    float float_4;

};

Texture2D tex_0 : register(t0);
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);
Texture2D tex_4 : register(t4);

SamplerState sam_0 : register(s0);

struct VS_IN
{
    float3 pos : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    //어떤 정점이든, 매트릭스 WVP를 적용시키겠다.
    output.pos = mul(float4(input.pos, 1.f), matWVP);
    //output.color = input.color;
    output.uv = input.uv;

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = tex_0.Sample(sam_0, input.uv);
    return color;
}