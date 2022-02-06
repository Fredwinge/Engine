
cbuffer Cbuf
{
	float4 face_colors[8];
};

float4 main( uint triangleID : SV_PrimitiveID) : SV_Target
{

	return float4(face_colors[(triangleID / 2) % 8].rgb, 1.0f);
}