#pragma once

#include "camera.hpp"
#include "renderer3d.hpp"
#include "GLFW/glfw3.h"

namespace Input{
	class InputManager
	{
	public:
		InputManager(GLFWwindow& _window, Graphic::Camera& _camera, Graphic::Renderer& _renderer);

		void Process(float _deltaTime);

	private:
		GLFWwindow& m_window;
		Graphic::Camera& m_camera;
		Graphic::Renderer& m_renderer;
	};
}