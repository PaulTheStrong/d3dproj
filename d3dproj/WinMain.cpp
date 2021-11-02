#include "Window.h"
#include <sstream>
#include "App.h"

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow
) {
	try {
		App app;
		app.Go();
	}
	catch (EngineException& e) 
	{
		const char* what = e.what();
		MessageBoxA(nullptr, what, e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (std::exception& e) 
	{
		MessageBoxA(nullptr, e.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
}