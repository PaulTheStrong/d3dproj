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
	wnd.Gfx().DrawTestTriangle();
	wnd.Gfx().EndFrame();
}