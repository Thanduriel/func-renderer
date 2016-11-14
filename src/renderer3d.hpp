#pragma once

#include "mesh.hpp"
#include "camera.hpp"

namespace Graphic{

	enum class RenderModes{
		Lines,
		Triangles
	};

	class Renderer
	{
	public:
		Renderer();

		void draw(GLFWwindow* _window);

		void AddMesh(Mesh* _mesh);

		Camera& GetCamera() { return m_camera; }

		void setRenderMode(RenderModes _mode);
	private:

		Camera m_camera;

		std::vector<Mesh*> m_meshes;
	};
}