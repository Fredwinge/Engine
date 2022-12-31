cbuffer CBuf
{
    float4x4 worldViewProj;
    float4x4 worldMatrix;
    float4 lightColor;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : color0;
    float3 normal : normal;
};

VS_OUTPUT main(VS_INPUT IN)
{

    VS_OUTPUT OUT;
	
    OUT.position = mul(float4(IN.position, 1.0f), worldViewProj);
    OUT.color = lightColor;
    OUT.normal = normalize(mul(IN.normal, (float3x3) worldMatrix));

    return OUT;
}