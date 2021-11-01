#include "Window.h"

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow
) {

	Window wnd(800, 300, L"Game engine");
	Window wnd2(100, 300, L"Game engin2");
	Window wnd3(800, 500, L"Game engine3");
	Window wnd4(800, 300, L"Game engine4");
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