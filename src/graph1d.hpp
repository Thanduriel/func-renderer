#pragma once

#include "mesh.hpp"
#include "function.hpp"

namespace Graphic{
	class Graph1d : public Mesh
	{
	public:
		Graph1d(Math::Function1D _func, float _res = 0.1f, float _size = 10.f);
	};
}