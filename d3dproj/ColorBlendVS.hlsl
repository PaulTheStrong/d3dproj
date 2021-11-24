cbuffer CBuf
{
	matrix transform;
};

struct VSOut
{
	float4 color : Color;
	float4 pos : SV_Position;
};

VSOut main(float3 position : Position, float4 color : Color)
{
	VSOut vso;
	vso.color = color;
	vso.pos = mul(float4(position, 1.0f), transform);
	return vso;
}