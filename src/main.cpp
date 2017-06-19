#include <iostream>
#include <time.h>
#include <string>

#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include <GL/GL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "scene.hpp"
#include "globals.hpp"
#include <fstream>
#include "functions1d.hpp"
#include "functions2d.hpp"
#include "modifiers.hpp"


void printP(std::fstream& file, int x, int y)
{
	file << "P( ";
	file << x;
	file << " ";
	file << y;
	file << " )" << std::endl;
}

int main(void)
{
	Math::PerlinNoise2D base(0, 0.02f, 0x12345);
	Math::PerlinNoise2D large(0, 0.05f, 0x23456);
	Math::PerlinNoise2D medium(0, 0.1f, 0x34567);
	Math::PerlinNoise2D small(0, 0.2f, 0x45678);
	Math::PerlinNoise2D mini(0, 0.4f);
	Math::PerlinNoise2D mini2(0, 0.8f);
	Math::PerlinNoise2D mini3(0, 1.6f);
	Math::PerlinNoise2D mini4(0, 3.2f);
	Math::ClampFunction clamp(0.5f, 1.f);
	Math::DistanceFunction2D distF;
	Math::AbsFunction absF;
	Math::InvFunction invF;
	Math::DistortFunction distortF;
	Math::SqrFunction sqrF;
	auto f = 
		distF[distortF]
		+ invF[absF[base]] * 20.f
		+ invF[absF[large]] * 10.f
		+ invF[absF[medium]] * 5.f
		+ invF[absF[small]] * 2.5f
		+ (invF[absF[mini]] * 1.25f
			+ invF[absF[mini2]] * 0.75f
			+ invF[absF[mini3]] * 0.35f
			+ invF[absF[mini4]] * 0.12f) /** clamp[(distF[distortF]+0.1f) * 0.1f]*/
		+ -30.f;
/*	std::fstream file("data.txt", std::fstream::out);
	int last = (int)f(Math::AVec2(0.f, 0.f));
	for (int i = 0; i < 119; ++i)
	{
		int y = (int)std::round(f(Math::AVec2((float)i, 0.f)));
		for(int j = std::min(y, last); j <= std::max(y,last); ++j)
			printP(file, i, j);
		last = y;
	}
	last = (int)f(Math::AVec2(0.f, 5.f));
	for (int i = 0; i < 119; ++i)
	{
		int y = (int)std::round(f(Math::AVec2((float)i * 0.3f, 5.f)));
		for (int j = std::min(y, last); j <= std::max(y, last); ++j)
			printP(file, i, j+7);
		last = y;
	}*/

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

	SceneSimplePerlin scene(window);
	scene.build();

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