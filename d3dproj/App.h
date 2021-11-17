#pragma once
#include "Window.h"
#include "EngineTimer.h"
#include "Box.h"

class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
	EngineTimer timer;
	std::vector<std::unique_ptr<Drawable>> objects;
};

