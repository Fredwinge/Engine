struct PS_INPUT
{
    float4 position : SV_Position;
    float4 color : color0;
    float3 normal : normal;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

cbuffer buffer : register(b0)
{
    float4 camDirToCenter;
};

PS_OUTPUT main(PS_INPUT IN)
{
    PS_OUTPUT OUT;
    
    float fade = dot(IN.normal.xyz, camDirToCenter.xyz);
    OUT.color = float4(IN.color.xyz, fade);
	
    return OUT;
}