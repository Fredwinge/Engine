

cbuffer CBuf
{
    float4x4 worldMatrix;
	float4x4 worldViewProj;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float2 uv		: TEXCOORD;
	float3 normal	: NORMAL;
	float3 binormal	: BINORMAL;
	float3 tangent	: TANGENT;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 uv		: TexCoord;
	float3 normal	: Normal;
    float3 worldPosition : World_Position;
};

VS_OUTPUT main(VS_INPUT IN)
{

	VS_OUTPUT OUT;
	
    OUT.position = mul(float4(IN.position, 1.0f), worldViewProj);
    OUT.worldPosition = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
    OUT.uv = IN.uv;
    
    float3 normal = mul(IN.normal, (float3x3)worldMatrix);
    OUT.normal = normalize(normal);

    return OUT;
}