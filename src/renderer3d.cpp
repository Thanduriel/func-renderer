#include "renderer3d.hpp"
#include "config.hpp"
#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include <GL/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "effect.hpp"
#include "texture.hpp"

namespace Graphic{

	Renderer::Renderer():
		m_camera(c_cameraLocation, c_cameraOrigin, glm::radians(60.f)),
		m_renderMode(RenderModes::Simple)
	{
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

//		glEnable(GL_CULL_FACE);
	}

	// ********************************************************************* //
	void Renderer::setRenderMode(RenderModes _mode)
	{
		m_renderMode = _mode;
		if(_mode == RenderModes::WireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Renderer::AddMesh(Mesh* _mesh)
	{
		m_meshes.push_back(_mesh);
		_mesh->updateNormals();
		_mesh->indexVBO();

		_mesh->GetVertices().upload();
		_mesh->GetNormals().upload();
		_mesh->GetIndices().upload();
	}

	// ********************************************************************* //
	void Renderer::draw(GLFWwindow* _window)
	{
		static Graphic::Effect effectTextured("shader/textured");
		static Graphic::Effect effectSimple("shader/simple");
		Effect& effect = m_renderMode == RenderModes::Simple ? effectSimple : effectTextured;

		glClearColor(0.0f, 1.0f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(effect.getProgId());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);


		GLuint MatrixID = glGetUniformLocation(effect.getProgId(), "MVP");
		GLuint mmID = glGetUniformLocation(effect.getProgId(), "M");
		GLuint colorId = glGetUniformLocation(effect.getProgId(), "uColor");
		GLuint lightDir = glGetUniformLocation(effect.getProgId(), "ulightDirection");
		GLuint textureSamp = glGetUniformLocation(effect.getProgId(), "utextureSampler");
		GLuint textureSamp2 = glGetUniformLocation(effect.getProgId(), "utextureSampler2");

		static Texture texture("texture/grass.DDS");
		static Texture textureRock("texture/rock.DDS");
#ifndef MODE2D
		glUniform4f(lightDir, 0.4472f, 0.8944f, 0.f, 0.f);
#else
		glUniform4f(lightDir, 0.f, 0.f, -1.f, 0.f);
#endif

		for (auto& mesh : m_meshes)
		{
			const VertexBuffer<>& vb = mesh->GetVertices();

			//set uniforms of the model
			glm::mat4 mvp = m_camera.GetViewProjection() * mesh->GetModelMatrix();
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(mmID, 1, GL_FALSE, &mesh->GetModelMatrix()[0][0]);
			uint32_t col = mesh->GetColor();
			glUniform4f(colorId, uint8_t(col >> 24) / 255.f, uint8_t(col >> 16) / 255.f, uint8_t(col >> 8) / 255.f, uint8_t(col) / 255.f);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.getId());
			// Texture Unit 0
			glUniform1i(textureSamp, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textureRock.getId());
			// Texture Unit 1
			glUniform1i(textureSamp2, 1);

			glBindBuffer(GL_ARRAY_BUFFER, vb.getId());
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetNormals().getId());
			glVertexAttribPointer(
				1,
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndices().getId());
			// Draw the triangles !
			glDrawElements(
				GL_TRIANGLES,      // mode
				mesh->GetIndices().size(),    // count
				GL_UNSIGNED_INT,   // type
				(void*)0           // element array buffer offset
			);
			
		//	glDrawArrays(GL_TRIANGLES, 0, vb.size());
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}