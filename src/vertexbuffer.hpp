#pragma once

#include <vector>
#include <gl/glew.h>
#include "GLFW/glfw3.h"
#include <gl/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace Graphic{
	class VertexBuffer : public std::vector<glm::vec3>
	{
	public:
		VertexBuffer() : m_isDirty(true)
		{
			glGenBuffers(1, &m_id);
		//	glBindBuffer(GL_ARRAY_BUFFER, m_id);
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		}

		bool isDirty() const { return m_isDirty; }

		int getId() const { return m_id; }
	private:
		bool m_isDirty;

		GLuint m_id;
	};
}