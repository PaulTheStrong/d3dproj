#pragma once
#include "Window.h"
#include "EngineTimer.h"
#include "Box.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

class App
{
public:
	App();
	~App();
	int Go();
private:
	void DoFrame();
	void SpawnSimulationWindow() noexcept;
	void SpawnBoxWindowManagerWindow() noexcept;
	void SpawnBoxWindows() noexcept;
private:
	ImguiManager imgui;
	Window wnd;
	EngineTimer timer;
	std::vector<std::unique_ptr<Drawable>> objects;
	float speed_factor = 0.0f;
	int objectCount = 1;
	Camera cam;
	PointLight light;
	std::vector<class Box*> boxes;
	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
	//std::wstring* models = { L"cup.obj", L"susanne.obj", L"house.obj"}
};

