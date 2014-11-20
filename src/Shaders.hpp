#pragma once

#include <string>

namespace cubedemo
{
	const std::string CUBE_SHADER_VERTEX = R"glsl(
#version 330

in vec3 position;
in vec3 normal;

out vec3 eyespaceNormal;
out vec3 diffuseColor;

uniform mat4 MVP;
uniform mat3 NormalMatrix;
uniform vec3 DiffuseColor;
uniform samplerBuffer InstancePositions;

void main()
{
    eyespaceNormal = NormalMatrix * normal;
    diffuseColor = DiffuseColor;
    
	vec4 position4 = vec4(position, 1.0);
	vec4 offset = texelFetch(InstancePositions, gl_InstanceID);
	vec4 offsetPosition = position4 + offset;
	gl_Position = MVP * offsetPosition;
}
)glsl";

	const std::string CUBE_SHADER_FRAGMENT = R"glsl(
#version 330

in vec3 eyespaceNormal;
in vec3 diffuseColor;

out vec4 fragment;

uniform vec3 AmbientColor;
uniform vec3 SpecularColor;
uniform float Shininess;
uniform vec3 LightPosition;

void main()
{
    vec3 N = normalize(eyespaceNormal);
    vec3 L = normalize(LightPosition);
    vec3 E = vec3(0, 1, 0);
    vec3 H = normalize(L + E);
    
    float df = max(0, dot(N, L));
    float sf = max(0, dot(N, H));
    sf = pow(sf, Shininess);
    
    vec3 color = AmbientColor + df * diffuseColor + sf * SpecularColor;
    
    fragment = vec4(color, 1);
}
)glsl";
}
