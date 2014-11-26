#version 330

in vec3 position;
in vec3 normal;

out vec3 fragPosition;
out vec3 fragNormal;
out float fragOpacity;

uniform samplerBuffer InstancePositions;
uniform samplerBuffer InstanceOpacities;
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
    fragOpacity = texelFetch(InstanceOpacities, gl_InstanceID).r;
    
	gl_Position = MVP * vec4(offsetPosition, 1.0);
}
