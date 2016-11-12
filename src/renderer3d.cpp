#include "renderer3d.hpp"
#include <gl/glew.h>

#include "GLFW/glfw3.h"
#include <gl/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "effect.hpp"

namespace Graphic{

	Renderer::Renderer():
		m_camera(glm::vec3(0.f, 0.f, -2.f), glm::vec3(0.f,0.f,0.f), glm::radians(60.f))
	{
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
	}

	// ********************************************************************* //
	void Renderer::setRenderMode(RenderModes _mode)
	{

	}

	// ********************************************************************* //
	void Renderer::updateBuffer(const VertexBuffer& _vb, int _layout)
	{
		if (!_vb.size()) return;
		glBindBuffer(GL_ARRAY_BUFFER, _vb.getId());
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, _vb.size() * sizeof(glm::vec3), &_vb[0], GL_STATIC_DRAW);

		glVertexAttribPointer(
			_layout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
	}

	// ********************************************************************* //
	void Renderer::draw(GLFWwindow* _window)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//todo build effect management
		static Graphic::Effect effect("shader/simple");
		glUseProgram(effect.getProgId());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		GLuint MatrixID = glGetUniformLocation(effect.getProgId(), "MVP");
		GLuint colorId = glGetUniformLocation(effect.getProgId(), "uColor");
		GLuint lightDir = glGetUniformLocation(effect.getProgId(), "ulightDirection");

#ifndef MODE2D
		glUniform4f(lightDir, 0.f, 1.f, 0.f, 0.f);
#else
		glUniform4f(lightDir, 0.f, 0.f, -1.f, 0.f);
#endif

		for (auto& mesh : m_meshes)
		{
			const VertexBuffer& vb = mesh->GetVertices();
			if (vb.isDirty()) { updateBuffer(vb); mesh->updateNormals(); }
			if (mesh->GetNormals().isDirty()) updateBuffer(mesh->GetNormals(), 1);

			//set uniforms of the model
			glm::mat4 mvp = m_camera.GetViewProjection() * mesh->GetModelMatrix();
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			uint32_t col = mesh->GetColor();
			glUniform4f(colorId, uint8_t(col >> 24) / 255.f, uint8_t(col >> 16) / 255.f, uint8_t(col >> 8) / 255.f, uint8_t(col) / 255.f);

	//		glBindBuffer(GL_ARRAY_BUFFER, vb.getId());
			
			glDrawArrays(GL_TRIANGLES, 0, vb.size());
		}
		glDisableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}