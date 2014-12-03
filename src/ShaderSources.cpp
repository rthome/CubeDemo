#include "ShaderSources.hpp"

#define LN(str) str "\n"

static const char *SHADER_SOURCE_CUBES_VERT = ""
LN("#version 410")
LN("")
LN("in vec3 position;")
LN("in vec3 normal;")
LN("")
LN("out vec3 fragPosition;")
LN("out vec3 fragNormal;")
LN("out float fragOpacity;")
LN("")
LN("uniform samplerBuffer InstancePositions;")
LN("uniform samplerBuffer InstanceOpacities;")
LN("uniform samplerBuffer InstanceScales;")
LN("uniform samplerBuffer InstanceRotations;")
LN("uniform mat4 ModelViewMatrix;")
LN("uniform mat4 ProjectionMatrix;")
LN("uniform mat4 MVP;")
LN("uniform mat3 NormalMatrix;")
LN("")
LN("vec3 quaternion_rotation(vec3 pos, vec4 quat)")
LN("{")
LN("    return pos + 2.0 * cross(cross(pos, quat.xyz) + quat.w * pos, quat.xyz);")
LN("}")
LN("")
LN("void main()")
LN("{")
LN("    vec3 instanceOffset = texelFetch(InstancePositions, gl_InstanceID).xyz;")
LN("    float instanceOpacity = texelFetch(InstanceOpacities, gl_InstanceID).r;")
LN("    float instanceScale = texelFetch(InstanceScales, gl_InstanceID).x;")
LN("    vec4 instanceRotation = texelFetch(InstanceRotations, gl_InstanceID);")
LN("")
LN("    vec3 offsetPosition = quaternion_rotation(position * instanceScale, instanceRotation) + instanceOffset;")
LN("")
LN("    fragNormal = normalize(NormalMatrix * quaternion_rotation(normal, instanceRotation));")
LN("    fragPosition = vec3(ModelViewMatrix * vec4(offsetPosition, 1.0));")
LN("    fragOpacity = instanceOpacity;")
LN("")
LN("    gl_Position = MVP * vec4(offsetPosition, 1.0);")
LN("}")
LN("");

static const char *SHADER_SOURCE_CUBES_FRAG = ""
LN("#version 410")
LN("")
LN("in vec3 fragPosition;")
LN("in vec3 fragNormal;")
LN("in float fragOpacity;")
LN("")
LN("out vec4 fragment;")
LN("")
LN("uniform vec3 LightPosition;")
LN("uniform vec3 LightIntensity;")
LN("uniform vec3 Kd;")
LN("uniform vec3 Ka;")
LN("uniform vec3 Ks;")
LN("uniform float Shininess;")
LN("uniform float Gamma;")
LN("")
LN("vec3 ads_light()")
LN("{")
LN("    vec3 n = normalize(fragNormal);")
LN("    vec3 s = normalize(LightPosition - fragPosition);")
LN("    vec3 v = normalize(-fragPosition);")
LN("    vec3 h = normalize(v + s);")
LN("")
LN("    vec3 A = Ka;")
LN("    vec3 D = Kd * max(dot(s, fragNormal), 0.0);")
LN("    vec3 S = Ks * pow(max(dot(h, n), 0.0), Shininess);")
LN("    vec3 L = LightIntensity * (A + D + S);")
LN("    return L;")
LN("}")
LN("")
LN("vec3 correct_gamma(vec3 color)")
LN("{")
LN("    return pow(color, vec3(1.0 / Gamma));")
LN("}")
LN("")
LN("void main()")
LN("{")
LN("    vec3 color = ads_light();")
LN("    fragment = vec4(correct_gamma(color), fragOpacity);")
LN("}")
LN("");

static const char *SHADER_SOURCE_BACKGROUND_VERT = ""
LN("#version 410")
LN("")
LN("in vec3 position;")
LN("in float brightness;")
LN("")
LN("out vec4 vertColor;")
LN("")
LN("uniform vec4 BaseColor;")
LN("uniform mat4 MVP;")
LN("")
LN("void main()")
LN("{")
LN("    vertColor = vec4(BaseColor.rgb, min(1, 1.5 * brightness));")
LN("    gl_Position = MVP * vec4(position, 1.0);")
LN("}")
LN("");

static const char *SHADER_SOURCE_BACKGROUND_FRAG = ""
LN("#version 410")
LN("")
LN("in vec4 vertColor;")
LN("")
LN("out vec4 fragment;")
LN("")
LN("uniform float Gamma;")
LN("")
LN("vec4 correct_gamma(vec4 color)")
LN("{")
LN("    return vec4(pow(color.rgb, vec3(1.0 / Gamma)), color.a);")
LN("}")
LN("")
LN("void main()")
LN("{")
LN("    fragment = correct_gamma(vertColor);")
LN("}")
LN("");

static const char *SHADER_SOURCE_HDRBLOOM_VERT = "";
static const char *SHADER_SOURCE_HDRBLOOM_FRAG = "";

const char* cubedemo::shaderSourceCubesVert()
{
    return SHADER_SOURCE_CUBES_VERT;
}

const char* cubedemo::shaderSourceCubesFrag()
{
    return SHADER_SOURCE_CUBES_FRAG;
}

const char* cubedemo::shaderSourceBackgroundVert()
{
    return SHADER_SOURCE_BACKGROUND_VERT;
}

const char* cubedemo::shaderSourceBackgroundFrag()
{
    return SHADER_SOURCE_BACKGROUND_FRAG;
}

const char* cubedemo::shaderSourceHDRBloomVert()
{
    return SHADER_SOURCE_HDRBLOOM_VERT;
}

const char* cubedemo::shaderSourceHDRBloomFrag()
{
    return SHADER_SOURCE_HDRBLOOM_FRAG;
}
