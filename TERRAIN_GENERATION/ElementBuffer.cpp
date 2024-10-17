#include "ElementBuffer.h"

#include <iostream>
ElementBuffer::ElementBuffer(std::vector<unsigned int>& indices)
    : 
    indices(indices),
    mIndiciesCount(indices.size())
      
{

    glGenBuffers(1, &EB_ID);

    
}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &EB_ID);

}


void ElementBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

}


void ElementBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void ElementBuffer::DrawIndexed()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


GLuint ElementBuffer::GetID()
{
    return EB_ID;
}

size_t ElementBuffer::GetIndiciesCount() const
{
    return mIndiciesCount;
}

