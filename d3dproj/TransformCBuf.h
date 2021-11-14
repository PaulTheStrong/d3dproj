#pragma once
#include "ConstantBuffers.h"
#include "GraphicsThrowMacro.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCBuf : public Bindable 
{
public:
	TransformCBuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;
};

