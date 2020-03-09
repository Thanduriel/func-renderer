#include <iostream>
#include <time.h>
#include <string>

#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include <GL/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <fstream>

#include "scenes/scene.hpp"
#include "globals.hpp"
#include "functions/functions1d.hpp"
#include "functions/functions2d.hpp"
#include "functions/modifiers.hpp"

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

//	SceneSimplePerlin scene(window);
//	Scene2D scene(window);
	SceneMountains scene(window);
//	scene.build();

	clock_t c = clock();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float frameTime = float(clock() - c) / CLOCKS_PER_SEC;
		c = clock();

		glfwSetWindowTitle(window, std::to_string(frameTime).c_str());
		
		scene.process(frameTime);
		scene.draw(window);
	}

	glDeleteVertexArrays(1, &VertexArrayID);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}