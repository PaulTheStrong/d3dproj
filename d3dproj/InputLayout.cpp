#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pPixelShaderBytecode)
{
	GetDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pPixelShaderBytecode->GetBufferPointer(),
		pPixelShaderBytecode->GetBufferSize(),
		&pInputLayout);
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
