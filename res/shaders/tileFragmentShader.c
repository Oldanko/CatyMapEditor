#version 430 core

in vec2 UV;

layout(location = 5) uniform sampler2D diffuse_map;
layout(location = 6) uniform int use_filter = 0;


out vec4 outColor;

void main()
{
    vec4 color = texture(diffuse_map, UV);
    if(use_filter > 0) color *= 0.7;
	outColor = vec4(color);
}
