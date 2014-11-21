#pragma once

#include <string>

// TODO: Put shader code into their own files
// Either embed in executable or load from file system
// CMake configure_file for that?

namespace cubedemo
{
	const std::string CUBE_SHADER_VERTEX = R"glsl(
#version 330

in vec3 position;
in vec3 normal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform samplerBuffer InstancePositions;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

void main()
{
	vec3 instanceOffset = texelFetch(InstancePositions, gl_InstanceID).xyz;
	vec3 offsetPosition = position + instanceOffset;

	fragNormal = normalize(NormalMatrix * normal);
	fragPosition = vec3(ModelViewMatrix * vec4(offsetPosition, 1.0));
	gl_Position = MVP * vec4(offsetPosition, 1.0);
}
)glsl";

	const std::string CUBE_SHADER_FRAGMENT = R"glsl(
#version 330

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 fragment;

uniform vec3 LightPosition;
uniform vec3 LightIntensity;
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;
uniform float Gamma;

vec3 ads_light()
{
	vec3 n = normalize(fragNormal);
	vec3 s = normalize(LightPosition - fragPosition);
	vec3 v = normalize(-fragPosition);
	vec3 h = normalize(v + s);
	
	vec3 A = Ka;
	vec3 D = Kd * max(dot(s, fragNormal), 0.0);
	vec3 S = Ks * pow(max(dot(h, n), 0.0), Shininess);
	vec3 L = LightIntensity * (A + D + S);
	return L;
}

vec3 correct_gamma(vec3 color)
{
	return pow(color, vec3(1.0 / Gamma));
}

void main()
{
	vec3 color = ads_light();
	fragment = vec4(correct_gamma(color), 1.0);
}
)glsl";
}
