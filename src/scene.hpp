#pragma once

#include "renderer3d.hpp"
#include "input.hpp"

class Scene
{
public:
	Scene(GLFWwindow* _window);

	void process(float _dTime);
	void draw(GLFWwindow* _window);
private:
	Input::InputManager m_inputManager;
	Graphic::Renderer m_renderer;
};