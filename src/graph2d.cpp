#include "graph2d.hpp"
#ifdef MULTITHREADED
#include <thread>
#endif

namespace Graphic{

	using namespace glm;
	using namespace Math;

	void Graph2D::buildSegment(int _begin, int _jump, int ind, int _indJump)
	{
		int test = ind;
		for (int ix = _begin; ix < m_size; ix+= _jump)
			for (int iy = 0; iy < m_size; iy++)
			{
				float fx = ix * m_resolution;
				float fy = iy * m_resolution;
				float ixr = fx + m_resolution;
				float iyr = fy + m_resolution;
				m_vertices[ind] = vec3(fx, m_function(AVec2(fx, fy)), fy);
				m_vertices[ind + 1] = vec3(fx, m_function(AVec2(fx, iyr)), iyr);
				m_vertices[ind + 2] = vec3(ixr, m_function(AVec2(ixr, fy)), fy);
				m_vertices[ind + 3] = vec3(ixr, m_function(AVec2(ixr, iyr)), iyr);
				m_vertices[ind + 4] = vec3(ixr, m_function(AVec2(ixr, fy)), fy);
				m_vertices[ind + 5] = vec3(fx, m_function(AVec2(fx, iyr)), iyr);

				ind += _indJump;
			}
	}

	Graph2D::Graph2D(const Math::Function2D& _func, float _res, float _size, uint32_t _color)
		: m_size(int(_size * 1.f / _res)), m_resolution(_res), m_function(_func)
	{
		m_vertices.clear();
		int scale = int(1.f / _res);
#ifndef MULTITHREADED
		m_vertices.reserve((int)_size * (int)_size * scale * scale * 6);

		for (float ix = 0; ix < _size; ix += _res)
			for (float iy = 0; iy < _size; iy += _res)
			{
				float ixr = ix + _res;
				float iyr = iy + _res;
				m_vertices.emplace_back(ix, _func(AVec2(ix,iy)), iy);
				m_vertices.emplace_back(ix, _func(AVec2(ix, iyr)), iyr);
				m_vertices.emplace_back(ixr, _func(AVec2(ixr, iy)), iy);
				m_vertices.emplace_back(ixr, _func(AVec2(ixr, iyr)), iyr);
				m_vertices.emplace_back(ixr, _func(AVec2(ixr, iy)), iy);
				m_vertices.emplace_back(ix, _func(AVec2(ix, iyr)), iyr);
			}
#else
		const size_t numVertices = m_size * m_size * 6;

		const unsigned numThreads = std::min(c_maxNumThreads, std::thread::hardware_concurrency());
		m_vertices.resize(numVertices /*+ numThreads * 6*/);
		// the intervalls only work well if this is the case
	//	assert(m_vertices.size() % numThreads == 0);
		const float interval = numThreads * _res;

		int indJump = numThreads * 6;
		std::vector<std::thread> threads;
		for (int i = 0; i < (int)numThreads - 1; ++i)
		{
			threads.emplace_back(&Graph2D::buildSegment, this, i, numThreads, i * 6, indJump);
		}

		buildSegment(numThreads-1, numThreads, (numThreads-1) * 6, indJump);

		for (auto& t : threads) t.join();
#endif
	}
}