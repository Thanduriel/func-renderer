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
		for (size_t i = 0; i < m_indices.size(); i += 3)
		{
			const uint32_t i0 = m_indices[i];
			const uint32_t i1 = m_indices[i+1];
			const uint32_t i2 = m_indices[i+2];
			vec3 edge1 = m_vertices[i1] - m_vertices[i0];
			vec3 edge2 = m_vertices[i2] - m_vertices[i0];
			vec3 normal = normalize(cross(edge1, edge2));
			m_normals[i0] += normal;
			m_normals[i1] += normal;
			m_normals[i2] += normal;
		}

		for (vec3& n : m_normals) n = glm::normalize(n);
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
}