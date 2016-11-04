#pragma once

#include "camera.hpp"
#include "GLFW/glfw3.h"

namespace Input{
	class InputManager
	{
	public:
		InputManager(GLFWwindow& _window, Graphic::Camera& _camera);

		void Process(float _deltaTime);

	private:
		GLFWwindow& m_window;
		Graphic::Camera& m_camera;
	};
}