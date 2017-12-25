#version 430 core

layout(location = 0) in vec2 tile_shift;

layout(location = 0) uniform float aspectR = 1.0f;
layout(location = 1) uniform vec3 camera = vec3(0.0f);
layout(location = 2) uniform vec2 chunk_shift = vec2(0.0f);

vec2 positions[] = {
	{0, 0},
	{0, 1},
	{1, 1},
	{1, 0},

};

void main()
{
	gl_Position = vec4(
		(
			positions[gl_VertexID%4]
			+ tile_shift
			+ chunk_shift
			- camera.xy
		)/camera.z * vec2(aspectR, 1.0),
		0.0f,
		1.0f
	);
}
