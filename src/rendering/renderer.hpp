#pragma once

#include "mesh.hpp"
#include "camera.hpp"
#include <memory>

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

		void addMesh(std::unique_ptr<Mesh> _mesh);

		Camera& getCamera() { return m_camera; }

		void setRenderMode(RenderModes _mode);
	private:
		RenderModes m_renderMode;
		Camera m_camera;

		std::vector<std::unique_ptr<Mesh>> m_meshes;
		Mesh* m_previousMesh;
	};
}