#ifndef LEVEL_H
#define LEVEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#define CHUNK_SIDE 12
#define CHUNK_SIZE 144

struct Grid
{
	GLuint vao, vbo;
	int size;
	Grid(int side);
	~Grid();
};

struct Chunk
{
	uint8_t tiles[CHUNK_SIDE][CHUNK_SIDE];
};




#endif
