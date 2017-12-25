#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "../include/SOIL.h"

#include "level.h"
#include "saver.h"
#include "controls.h"
#include "platform.h"


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
GLuint Texture(const char * path, GLuint wrap_s, GLuint wrap_t, GLuint min_filter, GLuint mag_filter);

GLFWwindow* window;

struct Screen
{
	int h = 480;
	int w = 640;
	float aspectR = 480.0f/640.0f;
} screen;

glm::vec3 camera = {0, 0, 8};
float camera_speed = 0.002;

struct
{
	double x = 0;
	double y = 0;
} mouse;

void update_mouse_position()
{
	glfwGetCursorPos(window, &mouse.x, &mouse.y);

	mouse.x = (mouse.x/screen.w * 2 - 1) / screen.aspectR;
	mouse.y = -mouse.y/screen.h * 2 + 1;

	mouse.x *= camera.z;
	mouse.x += camera.x;
	mouse.y *= camera.z;
	mouse.y += camera.y;
}

glm::vec4 texturelist[]
{
	{0.0, 0.5, 0.0, 0.5},
	{0.5, 1.0, 0.0, 0.5},
	{0.5, 1.0, 0.0, 0.25},
	{0.5, 1.0, 0.25, 0.5},
};
glm::vec4 positionlist[]
{
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 0.5},
	{0.0, 1.0, 0.5, 1.0},
};

glm::vec4 preview = { 0.5, 0.5, 0.5, 0.5 };

std::vector<Chunk> chunks;


int brush = 1;
bool isDrawing = false;
bool isErasing = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


void glfwError(int id, const char* description)
{
  std::cout << description << std::endl;
}

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++)
		printf("The arg is %s\n", argv[i]);

	printf("Kitty map editor\n");

	glfwSetErrorCallback(&glfwError);

    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screen.w, screen.h, "Level editor", NULL, NULL);
    if (!window)
    {
        printf("Could not create a window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	key_listener btn_Up(GLFW_KEY_W, false,
		[](){ camera.y += camera_speed * camera.z; }, nullptr, nullptr, nullptr);
	key_listener btn_Down(GLFW_KEY_S, false,
		[](){ camera.y -= camera_speed * camera.z; }, nullptr, nullptr, nullptr);
	key_listener btn_Left(GLFW_KEY_A, false,
		[](){ camera.x -= camera_speed * camera.z; }, nullptr, nullptr, nullptr);
	key_listener btn_Right(GLFW_KEY_D, false,
		[](){ camera.x += camera_speed * camera.z; }, nullptr, nullptr, nullptr);
	key_listener btn_add_chunk(GLFW_KEY_INSERT, false,
		nullptr, [](){ chunks.push_back(Chunk()); }, nullptr, nullptr);
	key_listener btn_delete_chunk(GLFW_KEY_DELETE, false,
		nullptr, [](){ if(chunks.size() > 0) chunks.pop_back(); }, nullptr, nullptr);

	key_listener btn_select_brush_0(GLFW_KEY_0, false,
		nullptr, [](){ brush = 0; }, nullptr, nullptr);
	key_listener btn_select_brush_1(GLFW_KEY_1, false,
		nullptr, [](){ brush = 1; }, nullptr, nullptr);
	key_listener btn_select_brush_2(GLFW_KEY_2, false,
		nullptr, [](){ brush = 2; }, nullptr, nullptr);
	key_listener btn_select_brush_3(GLFW_KEY_3, false,
		nullptr, [](){ brush = 3; }, nullptr, nullptr);

	key_listener btn_open_level(GLFW_KEY_S, true,
		nullptr, [](){ saveLevelBinary(chunks); }, nullptr, nullptr);
	key_listener btn_save_level(GLFW_KEY_O, true,
		nullptr, [](){ loadLevelBinary(chunks); }, nullptr, nullptr);

	GLuint texture = Texture(
		"res/textures.png",
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR
	);

	GLuint tex_platform = Texture(
		"res/platform.png",
		GL_REPEAT,
		GL_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR
	);



	sprite platform_small = 	{ {0.0f, 1.0f, 0.0f, 0.5f}, {0.0f, 2.0f, 0.0f, 1.0f} };
	sprite platform_medium = 	{ {0.0f, 1.5f, 0.0f, 0.5f}, {0.0f, 3.0f, 0.0f, 1.0f} };
	sprite platform_big = 		{ {0.0f, 2.0f, 0.0f, 0.5f}, {0.0f, 4.0f, 0.0f, 1.0f} };

	std::vector<float> v
		{
			1.0, 1.0, 1000,
			6.0, 1.0, 500
		};

	platform pl(v, &platform_big);

	GLuint program = LoadShaders("res/shaders/vertexShader.c", "res/shaders/fragmentShader.c");
	GLuint tileProgram = LoadShaders("res/shaders/tileVertexShader.c", "res/shaders/tileFragmentShader.c");
	GLuint circleProgram = LoadShaders("res/shaders/circleVertex.c", "res/shaders/circleFragment.c");

	glUseProgram(program);
	glUniform1f(0, screen.aspectR);

	glUseProgram(tileProgram);
	glUniform1f(0, screen.aspectR);
	glUniform1i(5, 0);

	glUseProgram(circleProgram);
	glUniform1f(0, screen.aspectR);

	glUseProgram(0);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	glClearColor(163.0/255, 188.0/255, 39.0/255, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Grid grid(CHUNK_SIDE);
	Grid tile(1);

	chunks.push_back(Chunk());


	auto drawPlatforms = [tex_platform](std::vector<platform*>& platforms)
	{
		glUniform1i(6, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_platform);
		for(auto p : platforms)
		{
			glUniform2f(2, p->position.x, p->position.y);
			glUniform4fv(3, 1, &p->s->size[0]);
			glUniform4fv(4, 1, &p->s->texCoords[0]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	};

	auto drawGrid = [](std::vector<Chunk>& chunks, Grid& grid)
	{
		glBindVertexArray(grid.vao);
		for(int i = 0; i < chunks.size(); i++)
		{
			glUniform2f(2, i*CHUNK_SIDE, 0);
			glDrawArraysInstanced(GL_LINE_STRIP, 0, 4, grid.size);
		}
	};

	auto drawTile = []()
	{
		glUniform1i(6, 1);
		glUniform2f(2, floor(mouse.x), floor(mouse.y));
		glUniform4fv(3, 1, &(positionlist[brush])[0]);
		glUniform4fv(4, 1, &(texturelist[brush])[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	};

	auto drawChunks = []()
	{
		for(int i = 0; i < chunks.size(); i++)
		{
			for(int y = 0; y < CHUNK_SIDE; y++)
				for(int x = 0; x < CHUNK_SIDE; x++)
				{
					glUniform2f(2, x + i*CHUNK_SIDE, y);
					glUniform4fv(3, 1, &positionlist[chunks[i].tiles[y][x]][0]);
					glUniform4fv(4, 1, &texturelist[chunks[i].tiles[y][x]][0]);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
		}
	};


	uint64_t frameCounter = 0;
	while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(tileProgram);
		glUniform3fv(1, 1, &camera[0]);
		glUniform1i(6, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		drawChunks();
		drawTile();
		drawPlatforms(platform::all);

		glUseProgram(program);
		glUniform3fv(1, 1, &camera[0]);
		glUniform4f(3, 0, 0, 0, 1);

		drawGrid(chunks, grid);

		glUseProgram(circleProgram);
		glUniform3fv(1, 1, &camera[0]);

		for(auto p : platform::all)
		{
			glUniform3fv(3, 1, &p->color[0]);
			for(auto wp : p->waypoints)
			{
				glUniform2fv(2, 1, &wp.position[0]);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 13);
			}
		}

		glUseProgram(0);
		glfwSwapBuffers(window);

		for(auto p : platform::all)
			p->update(frameCounter);


		glfwPollEvents();
		if(isDrawing & brush)
		{
			if(mouse.x > 0 && mouse.x < CHUNK_SIDE * chunks.size()
			&& mouse.y > 0 && mouse.y < CHUNK_SIDE)
			{
				chunks[int(mouse.x)/CHUNK_SIDE].tiles[int(mouse.y)][int(mouse.x)%CHUNK_SIDE] = isErasing ? 0 : brush;
			}
		}

		for(auto listener : key_listener::all)
		{
			listener->update();
		}

		update_mouse_position();

		++frameCounter;
		usleep(500);
    }

    glfwTerminate();

	glDeleteProgram(program);
	glDeleteProgram(tileProgram);
	glDeleteProgram(circleProgram);

    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
		isDrawing = true;
		else
		isDrawing = false;
	}
	else
	{
		if(action == GLFW_PRESS)
		{
			isDrawing = true;
			isErasing = true;
		}
		else
		{
			isDrawing = false;
			isErasing = false;
		}
	}
}

GLuint Texture(const char * path, GLuint wrap_s, GLuint wrap_t, GLuint min_filter, GLuint mag_filter)
{
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

	GLuint m_texture;
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

	if (wrap_s == GL_CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 0.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	glBindTexture(GL_TEXTURE_2D, 0);

	return m_texture;
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);

	if (VertexShaderStream.is_open())
	{
		std::string Line = "";

		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;

		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);

	if (FragmentShaderStream.is_open())
	{
		std::string Line = "";

		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;

		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();

	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling shader : %s\n", fragment_file_path);

	char const * FragmentSourcePointer = FragmentShaderCode.c_str();

	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
