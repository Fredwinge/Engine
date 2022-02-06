

cbuffer CBuf
{
	float4x4 worldViewProj;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float2 uv		: TEXCOORD;
	float3 normal	: NORMAL;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 uv		: TexCoord;
	float3 normal	: Normal;
};

VS_OUTPUT main(VS_INPUT IN)
{

	VS_OUTPUT OUT;
	
    OUT.position = mul(float4(IN.position, 1.0f), worldViewProj);
    OUT.uv = IN.uv;
    OUT.normal = IN.normal;

    return OUT;
}