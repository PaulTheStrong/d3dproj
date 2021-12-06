#pragma once
#include "Window.h"
#include "EngineTimer.h"
#include "Box.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"

class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	ImguiManager imgui;
	Window wnd;
	EngineTimer timer;
	std::vector<std::unique_ptr<Drawable>> objects;
	float speed_factor = 1.0f;
	int objectCount = 200;
	Camera cam;
	PointLight light;
};

