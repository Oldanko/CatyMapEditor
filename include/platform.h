#ifndef PLATFORM_H
#define PLATFORM_H

#include <vector>
#include "glm/glm.hpp"

#define RANDOM rand()/float(INT_MAX)

struct sprite
{
	glm::vec4 size;
	glm::vec4 texCoords;
};

struct waypoint
{
	static std::vector<waypoint*> all;
	glm::vec2 position;
	float steps;
	waypoint();
    waypoint(float x, float y, float s);
};

struct platform
{
	static std::vector<platform*> all;

	std::vector<waypoint> waypoints;
	glm::vec3 color;
	glm::vec2 position;
	glm::vec2 v;
	sprite * s;
	int stepsTotal = 0;
    platform(std::vector<float> &data, sprite * s);
    void update(uint64_t frame);
};


#endif
