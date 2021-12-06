#include "Mesh.h"
#include "BindableBase.h"
#include "GraphicsThrowMacro.h"
#include <fstream>
#include <sstream>


Mesh::Mesh(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material, std::wstring file)
	:
	TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};
		std::ifstream fin(file);
		std::string s;
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;
		int nidx = 0, iidx = 0;
		while (std::getline(fin, s))
		{
			if (!(s.starts_with("v") || s.starts_with("vn") || s.starts_with("f")))
			{
				continue;
			}
			std::istringstream iss(s);
			std::string type;
			iss >> type;
			if (type == "v")
			{
				vertices.emplace_back();
				float x, y, z;
				iss >> x >> y >> z;
				dx::XMStoreFloat3(&vertices.back().pos, { x, y, z });
			}
			else if (type == "vn")
			{
				float x, y, z;
				iss >> x >> y >> z;
				dx::XMStoreFloat3(&vertices[nidx++].n, { x, y, z });
			}
			else if (type == "f")
			{
				std::string f1, f2, f3;
				iss >> f1 >> f2 >> f3;
				std::istringstream if1(f1), if2(f2), if3(f3);
				std::string stringIndex;
				std::vector<unsigned int> fv1, fv2, fv3;
				while (std::getline(if1, stringIndex, '/'))
				{
					UINT vIndex = std::stoul(stringIndex);
					fv1.push_back(vIndex);
				}
				while (std::getline(if2, stringIndex, '/'))
				{
					UINT vIndex = std::stoul(stringIndex);
					fv2.push_back(vIndex);
				}
				while (std::getline(if3, stringIndex, '/'))
				{
					UINT vIndex = std::stoul(stringIndex);
					fv3.push_back(vIndex);
				}
				indices.push_back(fv1[0]);
				indices.push_back(fv2[0]);
				indices.push_back(fv3[0]);
			}
		}
		fin.close();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCBuf>(gfx, *this));

	struct PSMaterialConstant
	{
		alignas(16) dx::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} colorConst;
	colorConst.color = material;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) * TestObject::GetTransformXM() * dx::XMMatrixScaling(5.0f, 5.0f, 5.0f);
}
