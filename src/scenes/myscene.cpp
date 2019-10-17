#include "scene.hpp"
#include "functions/myfunctions.hpp"
#include "functions/mymodifiers.hpp"
#include "rendering/graph1d.hpp"
#include "rendering/graph2d.hpp"

using namespace Graphic;
using namespace Math;

void MyScene2D::build()
{
}


void MyScene3D::build()
{
	Math::PerlinNoise2D base(0, 0.1f, 0x12345);

	m_renderer.addMesh(std::make_unique<Graphic::Graph2D>(
		base * 18.f, 0.1f, c_worldSize));
}