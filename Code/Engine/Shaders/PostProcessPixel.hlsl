struct PS_INPUT
{
    float4 position : SV_Position;
    float2 uv : TexCoord;
    float3 normal : Normal;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

Texture2D g_SceneTexture;

SamplerState g_TexSampler;

PS_OUTPUT main(PS_INPUT IN)
{
    PS_OUTPUT Out;
    float4 color = g_SceneTexture.Sample(g_TexSampler, IN.uv);
    
    Out.color = float4(color.rgb, 1.0f);
    return Out;
}