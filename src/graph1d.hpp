#pragma once

#include "mesh.hpp"
#include "function.hpp"

namespace Graphic{
	class Graph1D : public Mesh
	{
	public:
		Graph1D(Math::Function1D _func, float _res = 0.1f, float _size = 10.f, uint32_t _color = 0x0000FFFF);
	};
}