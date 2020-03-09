#include "graph1d.hpp"

namespace Graphic{
	Graph1D::Graph1D(Math::Function1D _func, float _res, float _size, uint32_t _color)
	{
		m_color = _color;
		m_vertices.clear();
		m_vertices.reserve(static_cast<size_t>(_res * 2.f));
		m_indices.reserve(m_vertices.size());

		for (float i = 0.f; i < _size; i += _res)
		{
			const float val = _func(i);
			m_vertices.emplace_back(i, val, 0.f);
			m_vertices.emplace_back(i, _func(i) - 0.1f, 0.f);

	/*		m_vertices.emplace_back(i + _res, _func(i+_res), 0.f);
			m_vertices.emplace_back(i + _res, _func(i+_res)-0.1f, 0.f);
			m_vertices.emplace_back(i + _res, _func(i+_res)-0.1f, 0.f);
			m_vertices.emplace_back(i, _func(i), 0.f);*/
		}

		for (uint32_t i = 0; i < static_cast<uint32_t>(m_vertices.size())-2; i += 2)
		{
			m_indices.push_back(i);
			m_indices.push_back(i+2);
			m_indices.push_back(i+3);

			m_indices.push_back(i+3);
			m_indices.push_back(i+1);
			m_indices.push_back(i);
		}
	}
}