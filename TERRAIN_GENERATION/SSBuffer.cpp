#include "SSBuffer.h"

SSBuffer::SSBuffer()
{
    glGenBuffers(1, &SS_ID);
}

SSBuffer::~SSBuffer()
{
    glDeleteBuffers(1, &SS_ID);
}

void SSBuffer::Bind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SS_ID);
    //glBufferData(GL_SHADER_STORAGE_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SS_ID);
}