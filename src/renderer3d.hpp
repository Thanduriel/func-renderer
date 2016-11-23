#pragma once

#include "mesh.hpp"
#include "camera.hpp"

namespace Graphic{

	enum class RenderModes{
		Simple,
		Textured,
		WireFrame
	};

	class Renderer
	{
	public:
		Renderer();

		void draw(GLFWwindow* _window);

		void addMesh(Mesh* _mesh);

		Camera& getCamera() { return m_camera; }

		void setRenderMode(RenderModes _mode);
	private:
		RenderModes m_renderMode;
		Camera m_camera;

		std::vector<Mesh*> m_meshes;
	};
}