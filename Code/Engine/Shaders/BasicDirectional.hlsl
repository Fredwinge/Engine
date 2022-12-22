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

Texture2D g_SceneTexture : register(t0);
Texture2D g_SceneNormals : register(t1);
Texture2D g_ScenePosition : register(t2);

SamplerState g_TexSampler;

//TODO: Add support for multiple lights
PS_OUTPUT main(PS_INPUT IN)
{
    PS_OUTPUT Out;
    float4 Albedo = g_SceneTexture.Sample(g_TexSampler, IN.uv);
    float4 Normal = g_SceneNormals.Sample(g_TexSampler, IN.uv);
    float4 WorldPos = g_ScenePosition.Sample(g_TexSampler, IN.uv);
    
    float3 BaseAmbient = Albedo * 0.1;
    
    float3 lightDir = normalize(float3(0.0f, 10000.0f, 4000.0f) - WorldPos.xyz);
    float3 diffuse = max(dot(Normal.xyz, lightDir), 0.0);
    
    float3 color = (BaseAmbient + diffuse) * Albedo.xyz;
    
    Out.color = float4(color.rgb, 1.0f);
    return Out;
}