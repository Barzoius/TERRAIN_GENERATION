#pragma once

#include "Bindable.h"

#include <vector>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

template<typename VERTEX>
class VertexBuffer : public Bindable
{
public:
    VertexBuffer(const std::vector<VERTEX>& vertices)
        : sizeVERTEX(sizeof(VERTEX))
    {
        mVertices.reserve(vertices.size());

        for (const auto& vertex : vertices)
        {
            mVertices.push_back(vertex);  
        }

        glGenBuffers(1, &ID);

        std::cout << "VERT_BUF: " << ID << "\n";
    }

    ~VertexBuffer()
    {
        glDeleteBuffers(1, &ID);
    }

   // void Bind() override;

    void Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeVERTEX, mVertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


    }


    void Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //size_t GetVertexSize() const;


    size_t GetVertexSize() const
    {
        return sizeVERTEX;
    }

private:
    GLuint ID;
    size_t sizeVERTEX;
    std::vector<VERTEX> mVertices; 
};
