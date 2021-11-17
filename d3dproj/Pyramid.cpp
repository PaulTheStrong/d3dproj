#include "Pyramid.h"
#include "BindableBase.h"

Pyramid::Pyramid(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist) :
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
	if (!IsStaticInitialized()) {
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
			{ 0.0f,	 1.0f, 0.0f,	},
			{ -1.0f, -1.0f, -1.0f,	},
			{ 1.0f, -1.0f, -1.0f,	},
			{ 0.0f,	 -1.0f, 1.0f,	},
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
		auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
		AddStaticBind(std::move(pvs));

		const std::vector<unsigned short> indices =
		{
			0, 1, 2,
			2, 3, 0,
			3, 1, 0,
			1, 2, 3
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

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
				{1.0f, 0.3f, 0.7f},
				{1.0f, 0.5f, 0.2f},
				{1.0f, 0.0f, 0.0f},
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<ColorConstantBuffer>>(gfx, colorBuf));
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}


void Pyramid::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}

