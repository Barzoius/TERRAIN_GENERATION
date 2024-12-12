#pragma once

#include "Bindable.h"


#include <string>


class Texture : public Bindable
{
public:
    Texture(int width, int height);
    Texture(const Texture&) = delete;
    ~Texture();

    void Bind() override;
    void Unbind() override;

    void BindIMG(GLuint unit, GLenum access);

    void loadCubeMap();
    void loadFromFile(std::string_view);

    GLuint GetID();
    int GetWidth();
    int GetHeight();

    void SetParams();

private:
    int mTexWidth;
    int mTexHeight;

    GLuint texID;

};