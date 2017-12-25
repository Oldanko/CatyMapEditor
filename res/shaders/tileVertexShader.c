#version 430 core

layout(location = 0) uniform float aspectR = 1.0f;
layout(location = 1) uniform vec3 camera = vec3(0.0f);

layout(location = 2) uniform vec2 offset = vec2(0.0f);
layout(location = 3) uniform vec4 xy = {0.0f, 1.0f, 0.0f, 1.0f};
layout(location = 4) uniform vec4 uv = {0.0f, 1.0f, 0.0f, 1.0f};

out vec2 UV;

void main()
{
    UV = vec2(uv[gl_VertexID/2], uv[2+gl_VertexID%2]);

	gl_Position = vec4(
		(
            vec2(xy[gl_VertexID/2], xy[2+gl_VertexID%2])
			+ offset
			- camera.xy
		)/camera.z * vec2(aspectR, 1.0),
		0.0f,
		1.0f
	);
}
