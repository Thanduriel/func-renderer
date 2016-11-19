#include <iostream>
#include <time.h>
#include <string>

#include <gl/glew.h>

#include "GLFW/glfw3.h"
#include <gl/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "config.hpp"
#include "effect.hpp"
#include "globals.hpp"
#include "renderer3d.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "graph2d.hpp"
#include "graph1d.hpp"
#include "functions1d.hpp"
#include "functions2d.hpp"
#include "modifiers.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Config::g_windowWidth, Config::g_windowHeight, "World Renderer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		return -1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	Graphic::Renderer renderer;
	Input::InputManager inputManager(*window, renderer.GetCamera(), renderer);
	
#ifndef MODE2D
	Math::CubicIntFunction2D base(1501, 0.05f);
	Math::CubicIntFunction2D medium(1000, 0.2f);
	Math::CubicIntFunction2D small(1000, 1.0f);
	Math::LinearIntFunction linearf(1000);
	Math::ClampFunction clamp(0.1f, 6.f);
//	renderer.AddMesh(new Graphic::Graph2D(/*clamp<*/((16.f * base + medium * 4.3f + small * 0.5f) * (base * 2.f + 0.4f)), 0.1f, 100.f));
	Math::DistanceFunction2D distF;
	renderer.AddMesh(new Graphic::Graph2D(clamp<distF, 0.1f, 40.f));
#else
	Math::LinearIntFunction linearf(20);
	Math::CosIntFunction cosf(20);
	Math::PolynomIntFunction polf(20);
//	renderer.AddMesh(new Graphic::Graph1d(linearf));
//	renderer.AddMesh(new Graphic::Graph1d(cosf, 0.1f, 10.f, 0xFF0000FF));
//	renderer.AddMesh(new Graphic::Graph1d(cosf + linearf * 0.5f + (-0.5f), 0.1f, 10.f, 0x00FFFFFF));
	renderer.AddMesh(new Graphic::Graph1d(polf - cosf));

#endif
	//test -------------------------------------------------------------

	clock_t c = clock();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float frameTime = float(clock() - c) / CLOCKS_PER_SEC;
		c = clock();

		glfwSetWindowTitle(window, std::to_string(frameTime).c_str());
		
		inputManager.Process(frameTime);
		renderer.draw(window);
	}

	glfwTerminate();
	return 0;
}