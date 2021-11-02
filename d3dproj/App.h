#pragma once
#include "Window.h"
#include "EngineTimer.h"
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
};

