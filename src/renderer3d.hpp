#pragma once

#include "mesh.hpp"
#include "camera.hpp"

namespace Graphic{
	class Renderer
	{
	public:
		Renderer();

		void draw(GLFWwindow* _window);

		void AddMesh(Mesh* _mesh) { m_meshes.push_back(_mesh); updateBuffer(*_mesh); }

		Camera& GetCamera() { return m_camera; }
	private:
		void updateBuffer(Mesh& _mesh);

		Camera m_camera;

		std::vector<Mesh*> m_meshes;
	};
}