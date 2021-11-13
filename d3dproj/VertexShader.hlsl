cbuffer CBuf
{
	 matrix transform;
};

float4 main(float3 position : Position) : SV_Position
{
	return mul(transform, float4(position, 1.0f));
}