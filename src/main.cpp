#include <iostream>

#include <gl/glew.h>

#include "GLFW/glfw3.h"
#include <gl/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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

	Graphic::Renderer renderer;
	Input::InputManager inputManager(*window, renderer.GetCamera());
	Math::LinearIntFunction2D linearF2D(20);
//	renderer.AddMesh(new Graphic::Graph2D(linearF2D));
//	renderer.AddMesh(new Graphic::Mesh());
	Math::LinearIntFunction linearf(20);
	Math::CosIntFunction cosf(20);
	renderer.AddMesh(new Graphic::Graph1d(linearf));
	renderer.AddMesh(new Graphic::Graph1d(cosf + 0.5f, 0.1f, 10.f, 0xFF0000FF));
	renderer.AddMesh(new Graphic::Graph1d(cosf + linearf * 0.5f + (-0.5f), 0.1f, 10.f, 0x00FFFFFF));
	//test -------------------------------------------------------------


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		inputManager.Process(1.f / 60.f);
		renderer.draw(window);
	}

	glfwTerminate();
	return 0;
}