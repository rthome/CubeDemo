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
