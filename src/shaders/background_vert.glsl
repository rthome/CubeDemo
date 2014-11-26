#version 330

in vec3 position;
in vec3 brightness;

out vec4 vertColor;

uniform vec4 BaseColor;
uniform mat4 MVP;

void main()
{
	vertColor = abs(normalize(BaseColor + vec4(brightness, 1.0)));
	gl_Position = MVP * vec4(position, 1.0);
}
