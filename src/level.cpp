#include "level.h"

Grid::Grid(int side)
{
    size = side*side;
    glm::vec2 positions[size];
    for(int y = 0; y < side; y++)
    {
        int y_shift = y*side;
        for(int x = 0; x < side; x++)
        {
            positions[y_shift + x].x = x;
            positions[y_shift + x].y = y;
        }
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(glm::vec2) * size,
        positions,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    glVertexAttribDivisor(0, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Grid::~Grid(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
