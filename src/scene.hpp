#pragma once

#include "renderer3d.hpp"
#include "input.hpp"

class Scene
{
public:
	Scene(GLFWwindow* _window);

	void process(float _dTime);
	void draw(GLFWwindow* _window);

protected:
	Graphic::Renderer m_renderer;
private:
	Input::InputManager m_inputManager;
};

// ********************************************** //
// examples

//some 1d functions
class Scene2D : public Scene
{
public:
	Scene2D(GLFWwindow* _window);
};


// usage of multiple perlin noise functions with different ferquencies
class SceneSimplePerlin : public Scene
{
public:
	SceneSimplePerlin(GLFWwindow* _window);
};

class SceneMountains : public Scene
{
public:
	SceneMountains(GLFWwindow* _window);
};

// ********************************************** //
// custom scenes
class MyScene2D : public Scene
{
public:
	using Scene::Scene;

	void build();
};

class MyScene3D : public Scene
{
public:
	using Scene::Scene;

	void build();
};