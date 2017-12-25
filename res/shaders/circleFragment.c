#version 430 core

out vec4 outColor;

layout(location = 3)uniform vec3 color = vec3(1.0);

void main()
{
    outColor = vec4(color, 0.5);
}
