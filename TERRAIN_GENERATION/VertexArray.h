#pragma once

#include "Bindable.h"
#include "VertexBuffer.h"


class VertexArray : public Bindable
{
public:

    VertexArray();
    ~VertexArray();

    void Bind() override;
    void Unbind() override;


    GLuint GetID();
private:
    GLuint VA_ID;
   // VertexBuffer vbuf;
};