#include "App.h"
#include "Box.h"
#include "Pyramid.h"
#include <memory>
#include "Melon.h"
#include "GDIPlusManager.h"
#include "Surface.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "SkinnedPyramid.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Cylinder.h"
#include "AssetTest.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace dx = DirectX;

GDIPlusManager gdipm;

class DrawableFactory
{
public:
	DrawableFactory(Graphics& gfx) : gfx(gfx) {};

	std::unique_ptr<Drawable> operator()() {
		const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
		switch (typedist(rng)) {
		case 1:
			return std::make_unique<Box>(gfx, rng, adist, ddist, odist, rdist, bdist, mat);
		/*case 2:
			return std::make_unique<Melon>(gfx, rng, adist, ddist, odist, rdist, latdist, longdist);
		case 3:
			return std::make_unique<Pyramid>(gfx, rng, adist, ddist, odist, rdist, longdist);
		case 4:
			return std::make_unique<Sheet>(
				gfx, rng, adist, ddist,
				odist, rdist
				);
		case 5:
			return std::make_unique<SkinnedPyramid>(gfx, rng, adist, ddist, odist, rdist, longdist);
		case 6:
			return std::make_unique<SkinnedBox>(gfx, rng, adist, ddist, odist, rdist);*/
		case 2:
			return std::make_unique<Cylinder>(
				gfx, rng, adist, ddist, odist,
				rdist, bdist, tdist
				);
		case 3:
			return std::make_unique<Pyramid>(
				gfx, rng, adist, ddist, odist,
				rdist, tdist
				);
		case 4:
			return std::make_unique<SkinnedBox>(
				gfx, rng, adist, ddist,
				odist, rdist
				);
		case 5:
			return std::make_unique<AssetTest>(
				gfx, rng, adist, ddist,
				odist, rdist, mat, 0.5f
				);
		}
	}
private:
	Graphics& gfx;
	std::vector<class Box*> boxes;
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 0.0f, 3.1415f * 1.0f };
	std::uniform_real_distribution<float> ddist{ 0.0f, 3.1415f * 1.0f };
	std::uniform_real_distribution<float> odist{ 0.0f, 3.1415f * 0.02f };
	std::uniform_real_distribution<float> rdist{ 10.0f, 20.0f };
	std::uniform_int_distribution<int> latdist{ 3, 30 };
	std::uniform_int_distribution<int> longdist{ 3, 30 };
	std::uniform_int_distribution<int> typedist{ 5, 5 };
	std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
	std::uniform_int_distribution<int> tdist{ 3,30 };
	std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
};

App::App() :
	wnd(800, 600, L"Engine app"),
	light(wnd.Gfx()), comboBoxIndex(0)
{
	DrawableFactory drawableFactory{ wnd.Gfx() };
	
	objects.reserve(objectCount);
	std::generate_n(std::back_inserter(objects), objectCount, drawableFactory);

	for (auto& pd : objects)
	{
		if (auto pb = dynamic_cast<Box*>(pd.get()))
		{
			boxes.push_back(pb);
		}
	}

	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 200.0f));
}


void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	// render geometry
	for (auto& d : objects)
	{
		d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());

	// imgui windows
	SpawnSimulationWindow();
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	SpawnBoxWindowManagerWindow();
	SpawnBoxWindows();

	// present
	wnd.Gfx().EndFrame();
}

void App::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
}

void App::SpawnBoxWindowManagerWindow() noexcept
{
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;
		const auto preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box..."s;
		if (ImGui::BeginCombo("Box Number", preview.c_str()))
		{
			for (int i = 0; i < boxes.size(); i++)
			{
				const bool selected = comboBoxIndex && *comboBoxIndex == i;
				if (ImGui::Selectable(std::to_string(i).c_str(), selected))
				{
					comboBoxIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboBoxIndex)
		{
			boxControlIds.insert(*comboBoxIndex);
			comboBoxIndex.reset();
		}
	}
	ImGui::End();
}

void App::SpawnBoxWindows() noexcept
{
	for (auto i = boxControlIds.begin(); i != boxControlIds.end(); )
	{
		if (!boxes[*i]->SpawnControlWindow(*i, wnd.Gfx()))
		{
			i = boxControlIds.erase(i);
		}
		else
		{
			i++;
		}
	}
}

App::~App()
{}


int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessage())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}