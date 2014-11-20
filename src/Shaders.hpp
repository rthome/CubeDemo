#pragma once

#include <string>

namespace cubedemo
{
	const std::string CUBE_SHADER_VERTEX = R"glsl(
#version 330

in vec3 position;
in vec3 normal;

uniform mat4 MVP;
uniform samplerBuffer InstancePositions;

void main()
{
	vec4 position4 = vec4(position, 1.0);
	vec4 offset = texelFetch(InstancePositions, gl_InstanceID);
	vec4 offsetPosition = position4 + offset;
	gl_Position = MVP * offsetPosition;
}
)glsl";

	const std::string CUBE_SHADER_FRAGMENT = R"glsl(
#version 330

out vec4 fragment;
    
uniform vec4 CubeColor;

void main()
{
    fragment = CubeColor;
}
)glsl";
}
