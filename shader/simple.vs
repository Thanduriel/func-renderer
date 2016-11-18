#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal_modelspace;

uniform mat4 MVP;
uniform mat4 M;

out vec4 normal_cameraspace;
out vec4 positionWorldspace;

void main()
{
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
	positionWorldspace = M * vec4(vertexPosition_modelspace, 1.0);
	normal_cameraspace = /*MVP **/ vec4(normal_modelspace, 0.0);
}