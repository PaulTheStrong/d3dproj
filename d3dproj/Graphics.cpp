#include "Graphics.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_EXCEPT(hrcall) if (FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_DEVICE_REMOVED_EXCEPTION(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))

Graphics::Graphics(HWND hWnd) {

	DXGI_SWAP_CHAIN_DESC sd = {};
	auto bd = &sd.BufferDesc;
	bd->Width = 0;
	bd->Height = 0;
	bd->Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bd->RefreshRate.Numerator = 0;
	bd->RefreshRate.Denominator = 0;
	bd->Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bd->ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;
	GFX_EXCEPT(D3D11CreateDeviceAndSwapChain(
		nullptr, // let operation system decide graphics adapter,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));
	ID3D11Resource* pBackBuffer = nullptr;
	(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
	GFX_EXCEPT(pDevice->CreateRenderTargetView
	(
		pBackBuffer,
		nullptr,
		&pTarget
	));
	pBackBuffer->Release();
}

Graphics::~Graphics() {
	if (this->pContext != nullptr) 
	{
		pContext->Release();
	}
	if (this->pSwap != nullptr)
	{
		pSwap->Release();
	}
	if (this->pDevice != nullptr)
	{
		pDevice->Release();
	}
	if (this->pTarget != nullptr)
	{
		pTarget->Release();
	}
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = pSwap->Present(1, 0)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPTION(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			GFX_EXCEPT(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget, color);
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description]" << GetErrorDescription() << std::endl
		<< GetOriginString();
	std::string outputString = oss.str();
	whatBuffer = outputString.c_str();
	return whatBuffer.c_str();
}
const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphics hr exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return "aodjpapdsj error string";
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Engine graphics exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

#define ERROR_CODE_TO_MESSAGE(hr) (#hr)