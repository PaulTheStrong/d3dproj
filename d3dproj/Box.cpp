#include "Box.h"
#include "BindableBase.h"

Box::Box(Graphics& gfx, 
	std::mt19937& rng, 
	std::uniform_real_distribution<float>& adist, 
	std::uniform_real_distribution<float>& ddist, 
	std::uniform_real_distribution<float>& odist, 
	std::uniform_real_distribution<float>& rdist):
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	struct Vertex
	{
		struct {
			float x;
			float y;
			float z;
		} pos;
	};

	const std::vector<Vertex> vertices =
	{
		{ -1.0f, -1.0f, -1.0f,	},
		{ 1.0f,	 -1.0f, -1.0f,	},
		{ -1.0f, 1.0f, -1.0f,	},
		{ 1.0f,	 1.0f, -1.0f,	},
		{ -1.0f, -1.0f, 1.0f,	},
		{ 1.0f,	 -1.0f, 1.0f,	},
		{ -1.0f, 1.0f, 1.0f,	},
		{ 1.0f,	 1.0f, 1.0f,	},
	};
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
	AddBind(std::move(pvs));

	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

	//Create constant buffer for colors
	struct ColorConstantBuffer
	{
		struct {
			float r;
			float g;
			float b;
			float a;
		} faceColors[6];
	};

	const ColorConstantBuffer colorBuf =
	{
		{
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{1.0f, 1.0f, 1.0f},
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ColorConstantBuffer>>(gfx, colorBuf));
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}

