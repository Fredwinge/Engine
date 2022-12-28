//TODO: Make sure this matches c++ code
#define MAX_DIRECTIONAL_LIGHTS 8
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

struct DirectionalLight
{
    float4 LightColor;
    float4 LightDir;
};

struct PointLight
{
    float4 LightColor;
    float3 LightPos;
    float Range;
};

cbuffer LightBuffer : register(b0)
{
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
    PointLight pointLights[MAX_POINT_LIGHTS];
    
    float4 camPos;
};

Texture2D g_SceneTexture : register(t0);
Texture2D g_SceneNormals : register(t1);
Texture2D g_ScenePosition : register(t2);

SamplerState g_TexSampler;

void ComputeDirectionalLights(float3 Normal, float3 WorldPos, inout float3 Diffuse)
{
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i)
    {
        float3 lightDir = normalize(-directionalLights[i].LightDir.xyz);
        float diffFactor = saturate(dot(lightDir, Normal.xyz));
        //TODO: Add specular / shadows
        Diffuse += diffFactor * directionalLights[i].LightColor.xyz;
    }
}

void ComputePointLights(float3 Normal, float3 WorldPos, inout float3 Diffuse)
{
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        float3 lightDir = (pointLights[i].LightPos.xyz - WorldPos.xyz);
        if (length(lightDir) < pointLights[i].Range)
        {
            const float lightFade = 1.0f - (length(lightDir) / pointLights[i].Range);
            Diffuse += max(dot(Normal.xyz, normalize(lightDir)) * lightFade, 0.0) * pointLights[i].LightColor.xyz;
        }
    }
}

//TODO: Add support for multiple lights
PS_OUTPUT main(PS_INPUT IN)
{
    PS_OUTPUT Out;
    float4 Albedo = g_SceneTexture.Sample(g_TexSampler, IN.uv);
    float4 Normal = g_SceneNormals.Sample(g_TexSampler, IN.uv);
    float4 WorldPos = g_ScenePosition.Sample(g_TexSampler, IN.uv);
    
    //Uncompress normals, multiply by A to not apply light to background
    Normal.xyz = ((2.0f * Normal.xyz) - 1.0f) * Normal.a;
    
    //TODO: Add specular
    float3 Diffuse = 0;
    
    ComputeDirectionalLights(Normal.xyz, WorldPos.xyz, Diffuse);
    ComputePointLights(Normal.xyz, WorldPos.xyz, Diffuse);

    
    float3 BaseAmbient = Albedo * 0.1;
    float3 Color = (BaseAmbient + Diffuse) * Albedo.xyz;
    Color = lerp(Albedo.xyz, Color, Normal.a);
    
    Out.color = float4(Color.rgb, 1.0f);
    return Out;
}