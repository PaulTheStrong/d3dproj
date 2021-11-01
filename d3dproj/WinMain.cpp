#include "Window.h"

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow
) {

	Window wnd(800, 300, L"Game engine");
	
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gResult == -1) {
		return -1;
	}
	return msg.wParam;
}