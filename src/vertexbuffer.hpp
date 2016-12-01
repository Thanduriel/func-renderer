#pragma once

#include <vector>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <GL/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace Graphic{
	template<typename _T = glm::vec3, unsigned int _ArrayType = GL_ARRAY_BUFFER>
	class VertexBuffer : public std::vector<_T>
	{
		typedef std::vector<_T> ST;
	public:
		VertexBuffer() : m_isDirty(true)
		{
			glGenBuffers(1, &m_id);
		//	glBindBuffer(GL_ARRAY_BUFFER, m_id);
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		}
		~VertexBuffer()
		{
			glDeleteBuffers(1, &m_id);
		}

		bool isDirty() const { return m_isDirty; }

		int getId() const { return m_id; }

		void upload() const
		{
			glBindBuffer(_ArrayType, m_id);
			// Give our vertices to OpenGL.
			glBufferData(_ArrayType, ST::size() * sizeof(_T), &*ST::begin(), GL_STATIC_DRAW);
		}
	private:
		bool m_isDirty;

		GLuint m_id;
	};
}