#pragma once

#include "rendering/camera.hpp"
#include "rendering/renderer.hpp"
#include "GLFW/glfw3.h"

namespace Input{
	class InputManager
	{
	public:
		InputManager(GLFWwindow& _window, Graphic::Camera& _camera, Graphic::Renderer& _renderer);

		void process(float _deltaTime);

	private:
		GLFWwindow& m_window;
		Graphic::Camera& m_camera;
		Graphic::Renderer& m_renderer;
	};
}