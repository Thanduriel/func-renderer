#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal_modelspace;

uniform mat4 MVP;

out vec4 normal_cameraspace;

void main()
{
//	gl_Position.xyz = vertexPosition_modelspace;
//	gl_Position.w = 1.0;
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
	normal_cameraspace = /*MVP **/ vec4(normal_modelspace, 0.0);
}