#version 430 core

in vec2 UV;

layout(location = 3) uniform vec4 color;

out vec4 outColor;

void main()
{
	outColor = vec4(color);
}
