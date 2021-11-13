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
	static float z = 0;
	Mouse::Event event;;
	do {
		 event = wnd.mouse.Read();
		 if (event.GetType() == Mouse::Event::Type::WheelUp) {
			 z += 0.2;
		 }
		 if (event.GetType() == Mouse::Event::Type::WheelDown) {
			 z -= 0.2;
		 }
	} while (event.GetType() != Mouse::Event::Type::Invalid);
	angle += 0.01;
	wnd.Gfx().DrawTestTriangle(angle, x, 0, z);
	wnd.Gfx().DrawTestTriangle(-angle, x, 0, 5.0f);
	wnd.Gfx().EndFrame();
}