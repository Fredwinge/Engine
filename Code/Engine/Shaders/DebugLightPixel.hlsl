struct PS_INPUT
{
    float4 position : SV_Position;
    float4 color : color0;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

PS_OUTPUT main(PS_INPUT IN)
{
    PS_OUTPUT OUT;
    OUT.color = IN.color;
	
    return OUT;
}