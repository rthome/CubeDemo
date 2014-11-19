#pragma once

#include <string>

namespace cubedemo
{
	const std::string CUBE_SHADER_VERTEX = R"glsl(
#version 330

in vec3 position;
in vec3 normal;

out vec4 vertexColor;

uniform mat4 MVP;
uniform samplerBuffer InstancePositions;

void main()
{
	vertexColor = vec4(abs(normal), 1.0);

	vec4 position4 = vec4(position, 1.0);
	vec4 offset = texelFetchBuffer(InstancePositions, gl_InstanceID);
	vec4 offsetPosition = position4 + offset;
	gl_Position = MVP * offsetPosition;
}
)glsl";

	const std::string CUBE_SHADER_FRAGMENT = R"glsl(
#version 330

in vec4 vertexColor;

out vec4 fragment;

void main()
{
	fragment = vertexColor;
}
)glsl";
}
