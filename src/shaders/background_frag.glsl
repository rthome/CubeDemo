#version 330

flat in vec4 vertColor;

out vec4 fragment;

void main()
{
	fragment = vertColor;
}
