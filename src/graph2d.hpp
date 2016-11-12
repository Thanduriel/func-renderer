#pragma once

#include "function.hpp"
#include "mesh.hpp"

namespace Graphic{

	class Graph2D : public Mesh
	{
	public:
		Graph2D(const Math::Function2D& _func, float _res = 0.5f, float _size = 10.f, uint32_t _color = 0x0000FFFF);
	private:
	};

}