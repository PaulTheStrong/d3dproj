struct VSOut
{
	float4 color : Color;
	float4 pos : SV_Position;
};

cbuffer CBuf
{
	row_major matrix transform;
};

VSOut main(float3 position : Position, float4 color : Color)
{
	VSOut vso;
	vso.pos = mul(transform, float4(position.x, position.y, position.z, 1.0f));
	vso.color = color;
	return vso;
}