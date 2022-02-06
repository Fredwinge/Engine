
//cbuffer Cbuf
//{
//	float4 face_colors[8];
//};

Texture2D tex;
SamplerState Sampler;

float4 main( float2 uv : TexCoord) : SV_Target
{

	//return float4(face_colors[(triangleID / 2) % 8].rgb, 1.0f);

	return tex.Sample(Sampler, uv);
}