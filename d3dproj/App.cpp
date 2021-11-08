#include "App.h"
#include <sstream>

App::App() :
	wnd(800, 600, L"Engine app")
{}

int App::Go() {
	while (true)
	{
		if (const auto ecode = Window::ProcessMessage())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame() 
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	const float v = cos(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, v, 1.0);
	static float angle = 0;
	float x = wnd.mouse.GetPosX() / 400.0f - 1.0f;
	float y = -(wnd.mouse.GetPosY() / 300.0f - 1.0f);
	wnd.Gfx().DrawTestTriangle(angle += 0.1, x, y);
	wnd.Gfx().EndFrame();
}