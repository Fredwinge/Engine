
struct PS_INPUT
{
	float4 position : SV_Position;
	float2 uv		: TexCoord;
	float3 normal	: Normal;
    
    float3 worldPosition : World_Position;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    float4 normals : SV_Target1;
    float4 position : SV_Target2;
};

PS_OUTPUT main(PS_INPUT IN)
{
    PS_OUTPUT OUT;
    OUT.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    OUT.normals = float4(IN.normal.xyz, 1.0f);
    OUT.position = float4(IN.worldPosition.xyz, 1.0f);
	
    return OUT;
}