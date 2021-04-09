
struct PS_INPUT
{
	float4 position : SV_Position;
	float2 uv		: TexCoord;
	float3 normal	: Normal;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

PS_OUTPUT main(PS_INPUT IN)
{
    PS_OUTPUT OUT;
    OUT.color = float4(IN.normal.xyz, 1.0f);
	
    return OUT;
}