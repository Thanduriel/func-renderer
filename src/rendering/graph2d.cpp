#include "graph2d.hpp"
#include <chrono>
#include <iostream>
#ifdef MULTITHREADED
#include <thread>
#endif

namespace Graphic{

	using namespace glm;
	using namespace Math;

	void Graph2D::buildSegment(size_t _begin, size_t _end)
	{
		for (size_t iy = _begin; iy < _end; ++iy)
			for (size_t ix = 0; ix < m_size; ix++)
			{
				const float fx = ix * m_resolution;
				const float fy = iy * m_resolution;
				m_vertices[ix + iy*m_size] = vec3(fx, m_function(AVec2(fx, fy)), fy);
			}
	}

	Graph2D::Graph2D(const Math::Function2D& _func, float _res, float _size, uint32_t _color)
		: m_size(size_t(_size * 1.f / _res)), m_resolution(_res), m_function(_func)
	{
		const auto start = std::chrono::high_resolution_clock::now();
		m_vertices.clear();
		m_vertices.resize(m_size * m_size);
#ifndef MULTITHREADED
		buildSegment(0, m_size);
#else
		const size_t numThreads = std::min(c_maxNumThreads, std::thread::hardware_concurrency());
		const size_t numLines = m_size / numThreads;
		std::vector<std::thread> threads;
		for (int i = 0; i < numThreads - 1; ++i)
		{
			threads.emplace_back(&Graph2D::buildSegment, this, i* numLines, (i+1) * numLines);
		}
		// work for the main thread
		buildSegment((numThreads-1)* numLines, m_size);

		for (auto& t : threads) t.join();
#endif
		// construct triangles
		m_indices.reserve((m_size - 1) * (m_size - 1) * 2 * 3);
		for (int iy = 0; iy < m_size-1; ++iy)
			for (int ix = 0; ix < m_size-1; ix++)
			{
				const uint32_t s = static_cast<uint32_t>(m_size);
				assert(static_cast<size_t>(s) == m_size);

				const uint32_t v1 = ix + iy * s;
				const uint32_t v2 = ix + 1 + iy * s;
				const uint32_t v3 = ix + (iy + 1) * s;
				const uint32_t v4 = ix + 1 + (iy + 1) * s;
				m_indices.push_back(v1);
				m_indices.push_back(v3);
				m_indices.push_back(v2);
				m_indices.push_back(v4);
				m_indices.push_back(v2);
				m_indices.push_back(v3);
			}

		const auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Time to compute graph: " 
			<< std::chrono::duration<double>(end - start).count() 
			<< "s\n";
	}
}