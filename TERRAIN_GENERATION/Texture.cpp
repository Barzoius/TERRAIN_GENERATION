#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>


Texture::Texture(int width, int height)
    :
    mTexHeight(height), mTexWidth(width)
{
    glGenTextures(1, &texID);

}


Texture::~Texture()
{
    glDeleteTextures(1, &texID);
}

void Texture::Unbind()
{

}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::BindIMG(GLuint unit, GLenum access)
{
    glBindImageTexture(unit, texID, 0, GL_FALSE, 0, access, GL_RGBA32F);
}

void Texture::SetParams()
{
    //glCreateTextures(GL_TEXTURE_2D, 1, &texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Texture::loadFromFile(std::string_view filePath)
{
    int width, height, numChannels;
    stbi_set_flip_vertically_on_load(true);


    unsigned char* data = stbi_load(filePath.data(), &width, &height, &numChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

}


GLuint Texture::GetID()
{
    return texID;
}


int Texture::GetWidth()
{
    return  mTexWidth;
}


int Texture::GetHeight()
{
    return mTexHeight;
}