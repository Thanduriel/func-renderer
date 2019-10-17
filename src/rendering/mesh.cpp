#include <unordered_map>
#include "mesh.hpp"

namespace Graphic{

	using namespace glm;

	Mesh::Mesh():
		m_modelMatrix(1.f),
		m_color(0xBBBBBBFF)
	{
	}

	void Mesh::translate(glm::vec3 _dir)
	{
		glm::translate(m_modelMatrix, _dir);
	}

	void Mesh::updateNormals()
	{
		m_normals.resize(m_vertices.size());
		for (size_t i = 0; i < m_vertices.size(); i += 3)
		{
			vec3 edge1 = m_vertices[i+1] - m_vertices[i];
			vec3 edge2 = m_vertices[i + 2] - m_vertices[i];
			vec3 normal = normalize(cross(edge1, edge2));
			m_normals[i] = normal;
			m_normals[i+1] = normal;
			m_normals[i+2] = normal;

		}
	}

	struct KeyFuncs
	{
		size_t operator()(const vec3& _k) const
		{
			return std::hash<float>()(_k.x) ^ (std::hash<float>()(_k.y) >> 16)
				^ (std::hash<float>()(_k.z) << 16);
		}

		bool operator()(const vec3& _a, const vec3& _b) const
		{
			return _a == _b;
		}
	};

	void Mesh::indexVBO()
	{
		using NormalInd = std::pair<vec3, uint32_t>;

		std::unordered_map< vec3, NormalInd, KeyFuncs > indexedVerts;
		m_indices.reserve(m_vertices.size());

		uint32_t ind = 0;

		for (int i = 0; i < (int)m_vertices.size(); ++i)
		{
			auto it = indexedVerts.find(m_vertices[i]);
			if (it != indexedVerts.end())
			{
				// sum all coresponding normals to make them round
				it->second.first += m_normals[i];
				m_indices.push_back(it->second.second);
			}
			else // new vertex
			{
				indexedVerts[m_vertices[i]] = NormalInd(m_normals[i], ind);
				m_indices.push_back(ind);
				++ind;
			}
		}

		//write back required vertices
		m_vertices.resize(ind);
		m_normals.resize(ind);
		for (auto& el : indexedVerts)
		{
			m_vertices[el.second.second] = el.first;
			m_normals[el.second.second] = glm::normalize(el.second.first);
		}
	}
}