

cbuffer CBuf
{
	float4x4 transform;
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

	VS_OUTPUT vso;

	vso.position = mul(float4(IN.position, 1.0f), transform);
	vso.uv = IN.uv;
	vso.normal = IN.normal;

	return vso;
}