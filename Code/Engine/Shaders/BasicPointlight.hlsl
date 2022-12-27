//TODO: Make sure this matches c++ code
#define MAX_POINT_LIGHTS 8

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

struct PointLight
{
    float4 LightColor;
    float3 LightPos;
    float Range;
};

cbuffer LightBuffer : register(b0)
{
    PointLight pointLights[MAX_POINT_LIGHTS];
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
    
    //Uncompress normals, multiply by A to not apply light to background
    Normal.xyz = ((2.0f * Normal.xyz) - 1.0f) * Normal.a;
    
    float3 BaseAmbient = Albedo * 0.1;
    
    //TODO: Add specular
    float3 diffuse = 0;
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        float3 lightDir = (pointLights[i].LightPos.xyz - WorldPos.xyz);
        if (length(lightDir) < pointLights[i].Range)
        {
            diffuse += max(dot(Normal.xyz, normalize(lightDir)), 0.0) * pointLights[i].LightColor.xyz;
        }
    }
    
    float3 color = (BaseAmbient + diffuse) * Albedo.xyz;
    color = lerp(Albedo.xyz, color, Normal.a);
    
    Out.color = float4(color.rgb, 1.0f);
    return Out;
}