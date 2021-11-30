#pragma once

#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "BasicMath.h"

class Cone
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);
		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;
		for (int i = 0; i < longDiv; i++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * i)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}
		//Center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);

		//Tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);

		//Construct base
		std::vector<unsigned short> indices;
		for (int i = 0; i < longDiv; i++)
		{
			indices.push_back(iCenter);
			indices.push_back((i + 1) % longDiv);
			indices.push_back(i);
		}

		//Construct cone
		for (int i = 0; i < longDiv; i++)
		{
			indices.push_back(i);
			indices.push_back((i + 1) % longDiv);
			indices.push_back(iTip);
		}
		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
	template<class V>
	static IndexedTriangleList<V> MakeSkinned(int longDiv)
	{
		namespace dx = DirectX;


		assert(longDiv >= 3);
		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;
		for (int i = 0; i < longDiv * 2; i++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * (i / 2))
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().tex = { (float) (i / 2) / longDiv, 1.0f };
		}

		//Center
		unsigned short *centers = new unsigned short[longDiv];
		for (int i = 0; i < longDiv; i++) {
			vertices.emplace_back();
			vertices.back().pos = { 0.0f, 0.0f, -1.0f };
			vertices.back().tex = { 0.5f, 0.5f };
			centers[i] = (unsigned short)(vertices.size() - 1);
		}

		//Tip
		unsigned short *tips = new unsigned short[longDiv];
		for (int i = 0; i < longDiv; i++) 
		{
			vertices.emplace_back();
			vertices.back().pos = { 0.0f, 0.0f, 1.0f };
			tips[i] = (unsigned short)(vertices.size() - 1);
			vertices.back().tex = { 0.5f, 0.0f };
		}

		//Construct base
		std::vector<unsigned short> indices;
		for (int i = 0; i < longDiv; i++)
		{
			indices.push_back(centers[i]);
			indices.push_back((i * 2 + 2) % (longDiv * 2));
			indices.push_back((i * 2 + 1) % (longDiv * 2));
		}

		//Construct cone
		for (int i = 0; i < longDiv; i++)
		{
			indices.push_back((i * 2 + 1) % (longDiv * 2));
			indices.push_back((i * 2 + 2) % (longDiv * 2));
			indices.push_back(tips[i]);
		}

		delete[] tips;
		delete[] centers;

		return{
			std::move(vertices), std::move(indices)
		};
	}
};