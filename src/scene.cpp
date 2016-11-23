#include "scene.hpp"
#include "functions1d.hpp"
#include "functions2d.hpp"
#include "modifiers.hpp"
#include "graph1d.hpp"
#include "graph2d.hpp"
#include "globals.hpp"
#include "config.hpp"

Scene::Scene(GLFWwindow* _window)
	: m_renderer(),
	m_inputManager(*_window, m_renderer.GetCamera(), m_renderer)
{
#ifndef MODE2D
	Math::CubicIntFunction2D base(0, 0.02f, 0x12345);
	Math::CubicIntFunction2D large(0, 0.05f, 0x23456);
	Math::CubicIntFunction2D medium(0, 0.1f, 0x34567);
	Math::CubicIntFunction2D small(0, 0.2f, 0x45678);
	Math::CubicIntFunction2D mini(0, 0.4f);
	Math::CubicIntFunction2D mini2(0, 0.8f);
	Math::CubicIntFunction2D mini3(0, 1.6f);
	Math::CubicIntFunction2D mini4(0, 3.2f);
	Math::LinearIntFunction linearf(0);
	Math::ClampFunction clamp(0.5f, 20.f);
	//	renderer.AddMesh(new Graphic::Graph2D(/*clamp<*/((16.f * base + medium * 4.3f + small * 0.5f) * (base * 2.f + 0.4f)), 0.1f, 100.f));
	Math::DistanceFunction2D distF;
	Math::AbsFunction absF;
	Math::InvFunction invF;
	Math::DistortFunction distortF;
	Math::SqrFunction sqrF;
	m_renderer.AddMesh(new Graphic::Graph2D(
	//	distF[distortF] +
		invF[absF[base]] * 20.f
		+ invF[absF[large]] * 10.f
		+ (invF[absF[medium]] * 5.f
			+ invF[absF[small]] * 2.5f
			+ invF[absF[mini]] * 1.25f
			+ invF[absF[mini2]] * 0.75f
			+ invF[absF[mini3]] * 0.35f
			+ invF[absF[mini4]] * 0.12f) * sqrF[invF[absF[base]]]
		+ -20.f, 0.1f, c_worldSize));
#else
	Math::LinearIntFunction linearf(20);
	Math::CosIntFunction cosf(20);
	Math::PolynomIntFunction polf(20);
	//	renderer.AddMesh(new Graphic::Graph1d(linearf));
	//	renderer.AddMesh(new Graphic::Graph1d(cosf, 0.1f, 10.f, 0xFF0000FF));
	//	renderer.AddMesh(new Graphic::Graph1d(cosf + linearf * 0.5f + (-0.5f), 0.1f, 10.f, 0x00FFFFFF));
	renderer.AddMesh(new Graphic::Graph1d(polf - cosf));

#endif
}


void Scene::process(float _dTime)
{
	m_inputManager.Process(_dTime);
}

void Scene::draw(GLFWwindow* _window)
{
	m_renderer.draw(_window);
}