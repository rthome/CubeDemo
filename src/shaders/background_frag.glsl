#version 330

in vec4 vertColor;

out vec4 fragment;

uniform float Gamma;

vec4 correct_gamma(vec4 color)
{
	return vec4(pow(color.rgb, vec3(1.0 / Gamma)), color.a);
}

void main()
{
	fragment = correct_gamma(vertColor);
}
