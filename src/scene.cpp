#include "scene.hpp"
#include "functions1d.hpp"
#include "functions2d.hpp"
#include "modifiers.hpp"
#include "graph1d.hpp"
#include "graph2d.hpp"
#include "globals.hpp"
#include "config.hpp"

using namespace Math;

Scene::Scene(GLFWwindow* _window)
	: m_renderer(),
	m_inputManager(*_window, m_renderer.getCamera(), m_renderer)
{

}


void Scene::process(float _dTime)
{
	m_inputManager.process(_dTime);
}

void Scene::draw(GLFWwindow* _window)
{
	m_renderer.draw(_window);
}