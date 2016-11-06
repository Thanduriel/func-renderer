#include "graph1d.hpp"

namespace Graphic{
	Graph1d::Graph1d(Math::Function1D _func)
	{
		m_vertices.clear();
		m_vertices.reserve(10 * 2);

		for (float i = 0.f; i < 10.f; ++i)
		{
			m_vertices.emplace_back(i, _func(i), 0.f);
			m_vertices.emplace_back(i + 1.f, _func(i+1.f), 0.f);
			m_vertices.emplace_back(i + 1.f, _func(i+1.f)-0.1f, 0.f);
			m_vertices.emplace_back(i + 1.f, _func(i+1.f)-0.1f, 0.f);
			m_vertices.emplace_back(i, _func(i)-0.1f, 0.f);
			m_vertices.emplace_back(i, _func(i), 0.f);
		}
	}
}