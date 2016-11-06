#include "map.hpp"

namespace Graphic{

	using namespace glm;

	Map::Map(glm::ivec2 _size)
	{
		m_vertices.clear();
	/*	m_vertices.emplace_back(0.f, 0.f, 0.f);
		m_vertices.emplace_back(0.f, 0.f,1.f);
		m_vertices.emplace_back(1.f, 0.f, 0.f);
		m_vertices.emplace_back(1.f, 0.f, 1.f);
		m_vertices.emplace_back(1.f, 0.f, 0.f);
		m_vertices.emplace_back(0.f, 0.f, 1.f);*/
		m_vertices.reserve(_size.x * _size.y * 6);
		for (int ix = 0; ix < _size.x; ++ix)
			for (int iy = 0; iy < _size.y; ++iy)
			{
				float d = sqrt(ix + iy);

				m_vertices.emplace_back((float)ix, d, (float)iy);
				m_vertices.emplace_back((float)ix, d, (float)(iy + 1));
				m_vertices.emplace_back((float)(ix + 1), d, (float)iy);
				m_vertices.emplace_back((float)(ix + 1), d, (float)(iy + 1));
				m_vertices.emplace_back((float)(ix + 1), d, (float)iy);
				m_vertices.emplace_back((float)ix, d, (float)(iy + 1));
			}
	}
}