#include "controls.h"
#include <iostream>


std::vector<key_listener *> key_listener::all;

key_listener::key_listener()
{
    all.push_back(this);
}

void key_listener::update()
{
    if(id == -1) return;
    if((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) != ctrl) return;

    if(glfwGetKey(window, id) == GLFW_PRESS)
    {
        if(!pressed)
        {
            if(onPressed ) onPressed();
        }
        if(isPressed) isPressed();
        pressed = true;
    }
    else
    {
        if(!pressed)
        {
            if(onReleased) onReleased();
        }
        if(isReleased) isReleased();
        pressed = false;
    }
}

key_listener::key_listener(int id, bool ctrl,
    void(*isPressed)(),
    void(*onPressed)(),
    void(*isReleased)(),
    void(*onReleased)())
{
    all.push_back(this);
    this->id = id;
    this->ctrl = ctrl;
    this->isPressed = isPressed;
    this->onPressed = onPressed;
    this->isReleased = isReleased;
    this->onReleased = onReleased;
}
