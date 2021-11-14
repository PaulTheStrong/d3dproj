#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacro.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pPixelShaderBytecode);
	void Bind(Graphics& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};

