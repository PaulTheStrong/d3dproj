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
	const float t = timer.Peek();
	std::wostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}