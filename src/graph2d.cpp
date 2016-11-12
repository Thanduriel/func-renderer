#include "graph2d.hpp"

namespace Graphic{

	using namespace glm;
	using namespace Math;

	Graph2D::Graph2D(const Math::Function2D& _func, float _res, float _size, uint32_t _color)
	{
		m_vertices.clear();
	/*	m_vertices.emplace_back(0.f, 0.f, 0.f);
		m_vertices.emplace_back(0.f, 0.f,1.f);
		m_vertices.emplace_back(1.f, 0.f, 0.f);
		m_vertices.emplace_back(1.f, 0.f, 1.f);
		m_vertices.emplace_back(1.f, 0.f, 0.f);
		m_vertices.emplace_back(0.f, 0.f, 1.f);*/
		m_vertices.reserve((int)_size * (int)_size * (int)(1.f/ _res) * 6);
		for (float ix = 0; ix < _size; ix += _res)
			for (float iy = 0; iy < _size; iy += _res)
			{
				m_vertices.emplace_back((float)ix, _func(AVec2(ix,iy)), (float)iy);
				m_vertices.emplace_back((float)ix, _func(AVec2(ix, iy+_res)), (float)(iy + 1));
				m_vertices.emplace_back((float)(ix + 1), _func(AVec2(ix+_res, iy+_res)), (float)iy);
				m_vertices.emplace_back((float)(ix + 1), _func(AVec2(ix + _res, iy + _res)), (float)(iy + 1));
				m_vertices.emplace_back((float)(ix + 1), _func(AVec2(ix + _res, iy)), (float)iy);
				m_vertices.emplace_back((float)ix, _func(AVec2(ix, iy + _res)), (float)(iy + 1));
			}
	}
}