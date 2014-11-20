#pragma once

#include <string>

namespace cubedemo
{
	const std::string CUBE_SHADER_VERTEX = R"glsl(
#version 330

struct SHC{
    vec3 L00, L1m1, L10, L11, L2m2, L2m1, L20, L21, L22;
};

SHC groove = SHC(
    vec3( 0.3783264,  0.4260425,  0.4504587),
    vec3( 0.2887813,  0.3586803,  0.4147053),
    vec3( 0.0379030,  0.0295216,  0.0098567),
    vec3(-0.1033028, -0.1031690, -0.0884924),
    vec3(-0.0621750, -0.0554432, -0.0396779),
    vec3( 0.0077820, -0.0148312, -0.0471301),
    vec3(-0.0935561, -0.1254260, -0.1525629),
    vec3(-0.0572703, -0.0502192, -0.0363410),
    vec3( 0.0203348, -0.0044201, -0.0452180)
);

SHC beach = SHC(
    vec3( 0.6841148,  0.6929004,  0.7069543),
    vec3( 0.3173355,  0.3694407,  0.4406839),
    vec3(-0.1747193, -0.1737154, -0.1657420),
    vec3(-0.4496467, -0.4155184, -0.3416573),
    vec3(-0.1690202, -0.1703022, -0.1525870),
    vec3(-0.0837808, -0.0940454, -0.1027518),
    vec3(-0.0319670, -0.0214051, -0.0147691),
    vec3( 0.1641816,  0.1377558,  0.1010403),
    vec3( 0.3697189,  0.3097930,  0.2029923)
);

SHC tomb = SHC(
    vec3( 1.0351604,  0.7603549,  0.7074635),
    vec3( 0.4442150,  0.3430402,  0.3403777),
    vec3(-0.2247797, -0.1828517, -0.1705181),
    vec3( 0.7110400,  0.5423169,  0.5587956),
    vec3( 0.6430452,  0.4971454,  0.5156357),
    vec3(-0.1150112, -0.0936603, -0.0839287),
    vec3(-0.3742487, -0.2755962, -0.2875017),
    vec3(-0.1694954, -0.1343096, -0.1335315),
    vec3( 0.5515260,  0.4222179,  0.4162488)
);

in vec3 position;
in vec3 normal;

out vec4 vertexColor;

uniform mat4 MVP;
uniform samplerBuffer InstancePositions;

vec3 sh_light(in vec3 normal, SHC l){
    float x = normal.x;
    float y = normal.y; 
    float z = normal.z; 

	const float C1 = 0.429043;
    const float C2 = 0.511664;
    const float C3 = 0.743125;
    const float C4 = 0.886227;
    const float C5 = 0.247708;
    
    return (
        C1 * l.L22 * (x * x - y * y) +
        C3 * l.L20 * z * z +
        C4 * l.L00 -
        C5 * l.L20 +
        2.0 * C1 * l.L2m2 * x * y +
        2.0 * C1 * l.L21  * x * z +
        2.0 * C1 * l.L2m1 * y * z +
        2.0 * C2 * l.L11  * x +
        2.0 * C2 * l.L1m1 * y +
        2.0 * C2 * l.L10  * z
    );
}

void main()
{
	vertexColor = vec4(sh_light(normal, beach), 1.0);

	vec4 position4 = vec4(position, 1.0);
	vec4 offset = texelFetch(InstancePositions, gl_InstanceID);
	vec4 offsetPosition = position4 + offset;
	gl_Position = MVP * offsetPosition;
}
)glsl";

	const std::string CUBE_SHADER_FRAGMENT = R"glsl(
#version 330

in vec4 vertexColor;

out vec4 fragment;

void main()
{
	fragment = vertexColor;
}
)glsl";
}
