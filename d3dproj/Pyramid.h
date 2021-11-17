#pragma once
#include "DrawableBase.h"
class Pyramid : public DrawableBase<Pyramid>
{
public:
	Pyramid(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& addist,
		std::uniform_real_distribution<float>& dddist,
		std::uniform_real_distribution<float>& oddist,
		std::uniform_real_distribution<float>& rddist
	);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	//speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};

