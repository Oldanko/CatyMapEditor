#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>
#include <cstdint>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

struct key_listener
{
	static std::vector<key_listener *> all;

	int id = -1;
	bool ctrl = false;
	uint8_t pressed = 0;
	void (*isPressed)() = nullptr;
	void (*onPressed)() = nullptr;
	void (*isReleased)() = nullptr;
	void (*onReleased)() = nullptr;

	key_listener();
	key_listener(int id, bool ctrl,
		void(*isPressed)(),
		void(*onPressed)(),
		void(*isReleased)(),
		void(*onReleased)());
	void update();
};



#endif
