#version 430 core

layout(location = 0) uniform float aspectR = 1.0f;
layout(location = 1) uniform vec3 camera = vec3(0.0f);

layout(location = 2) uniform vec2 offset = vec2(0.0f);

vec2 positions[] =
{
    { 0.000000f, 1.000000f },
    { 0.500000f, 0.866026f },
    { 0.866025f, 0.500001f },
    { 1.000000f, 0.000001f },
    { 0.866026f, -0.499998f },
    { 0.500002f, -0.866024f },
    { 0.000003f, -1.000000f },
    { -0.499997f, -0.866027f },
    { -0.866024f, -0.500003f },
    { -1.000000f, -0.000004f },
    { -0.866028f, 0.499996f },
    { -0.500004f, 0.866023f }
};


void main()
{
	gl_Position = vec4(
		(
            positions[gl_VertexID%12]/5
			+ offset
			- camera.xy
		)/camera.z * vec2(aspectR, 1.0),
		0.0f,
		1.0f
	);
}
