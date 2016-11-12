#include "graph1d.hpp"

namespace Graphic{
	Graph1d::Graph1d(Math::Function1D _func, float _res, float _size, uint32_t _color)
	{
		m_color = _color;
		m_vertices.clear();
		m_vertices.reserve((int)_res * 6);

		for (float i = 0.f; i < _size; i += _res)
		{
			m_vertices.emplace_back(i, _func(i), 0.f);
			m_vertices.emplace_back(i + _res, _func(i+_res), 0.f);
			m_vertices.emplace_back(i + _res, _func(i+_res)-0.1f, 0.f);
			m_vertices.emplace_back(i + _res, _func(i+_res)-0.1f, 0.f);
			m_vertices.emplace_back(i, _func(i)-0.1f, 0.f);
			m_vertices.emplace_back(i, _func(i), 0.f);
		}
	}
}