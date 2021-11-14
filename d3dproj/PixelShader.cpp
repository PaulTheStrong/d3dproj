#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		0,
		&pPixelShader));
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0);

}

ID3DBlob* PixelShader::GetByteCode() const noexcept
{
	return pBytecodeBlob.Get();
}
