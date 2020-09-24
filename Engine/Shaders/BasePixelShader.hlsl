
struct PS_INPUT
{
	float4 position : SV_Position;
	float2 uv		: TexCoord;
	float3 normal	: Normal;
};

float4 main( PS_INPUT IN) : SV_Target
{

	return float4(IN.normal.xyz, 1.0f);
}