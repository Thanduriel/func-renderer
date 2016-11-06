#include "input.hpp"
#include "globals.hpp"

#include "GLFW/glfw3.h"

using namespace glm;

namespace Input{
	InputManager::InputManager(GLFWwindow& _window, Graphic::Camera& _camera) :
		m_camera(_camera),
		m_window(_window)
	{}

	void InputManager::Process(float _deltaTime)
	{
		double width_2 = Config::g_windowWidth / 2.0;
		double height_2 = Config::g_windowHeight / 2.0;
		double x, y;
		glfwGetCursorPos(&m_window, &x, &y);

		float dx = 0.05f * _deltaTime * float(width_2 - x);
		float dy = 0.05f * _deltaTime * float(height_2 - y);
		// mouse movement
		glfwSetCursorPos(&m_window, width_2, height_2);

		float speed = 10.f;
		glm::vec2 position(0.f);
		// Move forward
		if (glfwGetKey(&m_window,GLFW_KEY_W) == GLFW_PRESS){
			position += vec2(1.f, 0.f) * _deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(&m_window, GLFW_KEY_S) == GLFW_PRESS){
			position -= vec2(1.f, 0.f) * _deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(&m_window, GLFW_KEY_D) == GLFW_PRESS){
			position += vec2(0.f, 1.f) * _deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(&m_window, GLFW_KEY_A) == GLFW_PRESS){
			position -= vec2(0.f, 1.f) * _deltaTime * speed;
		}
		m_camera.update(dx, dy, position);

		//other options
		if (glfwGetKey(&m_window, GLFW_KEY_1))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (glfwGetKey(&m_window, GLFW_KEY_2))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (glfwGetKey(&m_window, GLFW_KEY_0))
			m_camera.reset();
	}
}