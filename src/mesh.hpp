#pragma once

#include "vertexbuffer.hpp"

namespace Graphic{

	class Mesh
	{
	public:
		Mesh();

		const VertexBuffer& GetVertices() const { return m_vertices; }
		const VertexBuffer& GetNormals() const { return m_normals; }

		const glm::mat4& GetModelMatrix() const { return m_modelMatrix; }
		uint32_t GetColor() const { return m_color; }

		//calculates the normals for the current vertices
		void updateNormals();
	protected:

		glm::mat4 m_modelMatrix;

		VertexBuffer m_vertices;
		VertexBuffer m_normals;

		uint32_t m_color;
	};

}