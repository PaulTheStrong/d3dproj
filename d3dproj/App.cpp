#include "App.h"
#include "Box.h"
#include "Pyramid.h"
#include <memory>

App::App() :
	wnd(800, 600, L"Engine app")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 1.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.05f);
	std::uniform_real_distribution<float> rdist(5.0f, 20.0f);
	for (auto i = 0; i < 20; i++)
	{
		objects.push_back(std::make_unique<Box>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.0f));
}

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
	char c = wnd.kbd.ReadChar();
	if (c == 'a') {
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 1.0f);
		std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 1.0f);
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.5f);
		std::uniform_real_distribution<float> rdist(25.0f, 40.0f);
			objects.push_back(std::make_unique<Box>(
				wnd.Gfx(), rng, adist,
				ddist, odist, rdist
				));
	}
	if (c == 's' && objects.size() != 0) {
		objects.pop_back();
	}
	auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& obj : objects)
	{
		obj->Update(dt);
		obj->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();
}