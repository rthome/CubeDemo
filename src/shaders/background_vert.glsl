#version 330

in vec3 position;

flat out vec4 vertColor;

uniform vec4 BaseColor;
uniform mat4 MVP;

void main()
{
	vertColor = BaseColor;
	gl_Position = MVP * vec4(position, 1.0);
}
