
struct PS_INPUT
{
	float4 position : SV_Position;
	float2 uv		: TexCoord;
	float3 normal	: NORMALOS;
};

float4 main( PS_INPUT IN) : SV_Target
{

    float3 color = IN.normal.xyz;
	
	return float4(color, 1.0f);
}