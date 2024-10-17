#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Bindable
{
public:
    virtual void Unbind() = 0;
    virtual void Bind() = 0;
    virtual ~Bindable() = default;

};

