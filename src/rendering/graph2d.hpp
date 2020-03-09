#pragma once

#include <atomic>

#include "functions/function.hpp"
#include "mesh.hpp"

namespace Graphic{

	class Graph2D : public Mesh
	{
	public:
		Graph2D(const Math::Function2D& _func, float _res = 0.5f, float _size = 10.f, uint32_t _color = 0x0000FFFF);
	private:
		void buildSegment(size_t _begin, size_t _end);

		size_t m_size;
		float m_resolution;
		Math::Function2D m_function;
	};

}