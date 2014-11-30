#version 330

in vec3 position;
in float brightness;

out vec4 vertColor;

uniform vec4 BaseColor;
uniform mat4 MVP;

void main()
{
	vertColor = vec4(BaseColor.rgb, min(1, 1.5 * brightness));
	gl_Position = MVP * vec4(position, 1.0);
}
