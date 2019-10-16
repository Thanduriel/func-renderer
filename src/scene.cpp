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
/*	Math::WorleyNoise base;
	Math::WorleyNoiseFunction func(base, 2);
	Math::WorleyNoiseFunction func1(base, 0);
	m_renderer.addMesh(new Graphic::Graph2D(func - func1, 0.1f, c_worldSize));*/

}


void Scene::process(float _dTime)
{
	m_inputManager.process(_dTime);
}

void Scene::draw(GLFWwindow* _window)
{
	m_renderer.draw(_window);
}

// *************************************************************** //
// examples

Scene2D::Scene2D(GLFWwindow* _window)
	: Scene(_window)
{
	Func1D::LinearIntFunction linearf(0);
	Func1D::CosIntFunction cosf(0);
	Func1D::PerlinNoise1D polf(0);
	m_renderer.addMesh(std::make_unique<Graphic::Graph1D>(linearf + -0.5f));
	m_renderer.addMesh(std::make_unique<Graphic::Graph1D>(cosf, 0.1f, 10.f, 0xFF0000FF));
	m_renderer.addMesh(std::make_unique<Graphic::Graph1D>(polf + 0.5f, 0.1f, 10.f, 0xFFFFFFFF));
}


SceneSimplePerlin::SceneSimplePerlin(GLFWwindow* _window)
	: Scene(_window)
{
	Math::PerlinNoise2D base(0, 0.02f, 0x12345);
	Math::PerlinNoise2D large(0, 0.05f, 0x23456);
	Math::PerlinNoise2D medium(0, 0.1f, 0x34567);
	Math::PerlinNoise2D small(0, 0.2f, 0x45678);

	m_renderer.addMesh(std::make_unique<Graphic::Graph2D>(
		base * 20.f
		+ large * 10.f
		+ medium * 5.f
		+ small * 2.5f
		+ -20.f, 0.1f, c_worldSize));
}


SceneMountains::SceneMountains(GLFWwindow* _window)
	: Scene(_window)
{
	Math::PerlinNoise2D base(0, 0.02f, 0x12345);
	Math::PerlinNoise2D large(0, 0.05f, 0x23456);
	Math::PerlinNoise2D medium(0, 0.1f, 0x34567);
	Math::PerlinNoise2D small(0, 0.2f, 0x45678);
	Math::PerlinNoise2D mini(0, 0.4f);
	Math::PerlinNoise2D mini2(0, 0.8f);
	Math::PerlinNoise2D mini3(0, 1.6f);
	Math::PerlinNoise2D mini4(0, 3.2f);
	Func1D::LinearIntFunction linearf(0);
	Math::ClampFunction clamp(0.5f, 1.f);
	Math::DistanceFunction2D distF;
	Math::AbsFunction absF;
	Math::InvFunction invF;
	Math::DistortFunction distortF;
	Math::SqrFunction sqrF;
	m_renderer.addMesh(std::make_unique<Graphic::Graph2D>(
		distF[distortF]
		+ invF[absF[base]] * 20.f
		+ invF[absF[large]] * 10.f
			+ invF[absF[medium]] * 5.f
			+ invF[absF[small]] * 2.5f
			+ (invF[absF[mini]] * 1.25f
			+ invF[absF[mini2]] * 0.75f
			+ invF[absF[mini3]] * 0.35f
			+ invF[absF[mini4]] * 0.12f) /** clamp[(distF[distortF]+0.1f) * 0.1f]*/
		+ -40.f, 0.1f, c_worldSize));
}