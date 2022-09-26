struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TexCoord;
    float3 normal : Normal;
};

VS_OUTPUT main(VS_INPUT IN)
{
    VS_OUTPUT Out;
    Out.position = float4(IN.position.xz, 0.0f, 1.0f);
    Out.uv = IN.uv;
    Out.normal = IN.normal;
	return Out;
}