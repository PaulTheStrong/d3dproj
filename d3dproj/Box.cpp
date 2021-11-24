#include "Box.h"
#include "BindableBase.h"
#include "Cube.h"

namespace dx = DirectX;

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
	if (!IsStaticInitialized()) {
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};

		auto model = Cube::make<Vertex>();
		model.Transform(dx::XMMatrixScaling(1.0f, 0.8f, 1.2f));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorIndexVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorIndexPS.cso"));
		AddStaticBind(std::move(pvs));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		//Create constant buffer for colors
		struct ColorConstantBuffer
		{
			struct {
				float r;
				float g;
				float b;
				float a;
			} faceColors[8];
		};

		const ColorConstantBuffer colorBuf =
		{
			{
				{0.8f, 0.8f, 0.8f},
				{0.9f, 1.0f, 0.1f},
				{0.5f, 0.0f, 1.0f},
				{1.0f, 0.3f, 0.7f},
				{0.3f, 0.5f, 0.2f},
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

