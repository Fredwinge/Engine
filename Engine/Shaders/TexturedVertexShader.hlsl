

cbuffer CBuf
{
	float4x4 transform;
};

struct VSOut
{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 uv : TexCoord)
{
	VSOut vso;

	vso.pos = mul(float4(pos.xyz, 1.0f), transform);
	vso.tex = uv;

	return vso;
}