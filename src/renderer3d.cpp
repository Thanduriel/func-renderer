#include "renderer3d.hpp"
#include <gl/glew.h>

#include "GLFW/glfw3.h"
#include <gl/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "effect.hpp"

namespace Graphic{

	Renderer::Renderer():
		m_camera(glm::vec3(4.f, 3.f, 3.f), glm::vec3(0.f,0.f,0.f), glm::radians(60.f))
	{
		
	}

	void Renderer::updateBuffer(Mesh& _mesh)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _mesh.GetVertices().getId());
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, _mesh.GetVertices().size() * sizeof(glm::vec3), &_mesh.GetVertices()[0], GL_STATIC_DRAW);
	}

	void Renderer::draw(GLFWwindow* _window)
	{
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//todo build effect management
		static Graphic::Effect effect("shader/simple");
		glUseProgram(effect.getProgId());
		glEnableVertexAttribArray(0);

		GLuint MatrixID = glGetUniformLocation(effect.getProgId(), "MVP");

		for (auto& mesh : m_meshes)
		{
			const VertexBuffer& vb = mesh->GetVertices();
			if (vb.isDirty()) updateBuffer(*mesh);

			glm::mat4 mvp = m_camera.GetViewProjection() * mesh->GetModelMatrix();

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

			glBindBuffer(GL_ARRAY_BUFFER, vb.getId());
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, vb.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
		}
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}