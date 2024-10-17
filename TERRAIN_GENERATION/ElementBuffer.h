#pragma once

#include "Bindable.h"
#include <vector>

class ElementBuffer : public Bindable
{
public:
    ElementBuffer(std::vector<unsigned int>& idices);
    ~ElementBuffer();

    void Bind() override;
    void Unbind() override;

    void DrawIndexed();

    GLuint GetID();

    size_t GetIndiciesCount() const;

private:
    GLuint EB_ID;
    std::vector<unsigned int> indices;

    size_t mIndiciesCount;
};