#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacro.h"

class VertexBuffer : public Bindable
{
public:
	template <class V>
	VertexBuffer (Graphics& gfx, const std::vector<V>& vertices)
		: stride(sizeof(V))
	{
		INFOMAN(gfx);
		//Create buffer description
		D3D11_BUFFER_DESC bd = { 0 };
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.ByteWidth = UINT(vertices.size() * sizeof(V));
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();

		//Create buffer for vertices to pass it to Input assembler stage
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}

	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
