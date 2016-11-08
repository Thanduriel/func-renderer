#pragma once

#include "vertexbuffer.hpp"

namespace Graphic{

	class Mesh
	{
	public:
		Mesh();

		const VertexBuffer& GetVertices() const { return m_vertices; }

		const glm::mat4& GetModelMatrix() const { return m_modelMatrix; }
		uint32_t GetColor() const { return m_color; }
	protected:
		glm::mat4 m_modelMatrix;

		VertexBuffer m_vertices;

		uint32_t m_color;
	};

}