#include <platform.h>

std::vector<waypoint*> waypoint::all;

waypoint::waypoint()
{
    all.push_back(this);
}

waypoint::waypoint(float x, float y, float s)
{
    all.push_back(this);
    position = {x, y};
    steps = s;
}

std::vector<platform*> platform::all;

platform::platform(std::vector<float> &data, sprite * s)
{
    all.push_back(this);

    this->s = s;
    color = glm::vec3(RANDOM, RANDOM, RANDOM);
    for(int i = 0; i < data.size();)
    {
        waypoints.push_back({data[i++],data[i++], data[i++]});
        stepsTotal += waypoints.back().steps;
    }
}

void platform::update(uint64_t frame)
{
    int step = frame % stepsTotal;
    int i;
    for(i = 0; i < waypoints.size(); i++)
    {
        int stepsMilestone = waypoints[i].steps;
        if(step <= stepsMilestone) break;
        step -= stepsMilestone;
    }
    v = (waypoints[(i+1)%waypoints.size()].position - waypoints[i].position)/waypoints[i].steps;
    position = waypoints[i].position + v*float(step);
}
