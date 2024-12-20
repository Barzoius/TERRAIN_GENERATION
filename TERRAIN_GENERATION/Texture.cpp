#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

uint32_t Texture::nextUnit = 0;

Texture::Texture(int width, int height, Specs specs)
    :
    mTexHeight(height), mTexWidth(width), mSpecs(specs), texUnit(nextUnit++)
{
    glGenTextures(1, &texID);

}

void Texture::SetActive()
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
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
    glBindTexture(mSpecs.TARGET, texID);
}

void Texture::AccessBind(GLenum access)
{
    glBindImageTexture(texUnit, texID, 0, GL_FALSE, 0, access, mSpecs.INTERNAL);
}

void Texture::SetParams()
{
    //glCreateTextures(GL_TEXTURE_2D, 1, &texID);
    glTexParameteri(mSpecs.TARGET, GL_TEXTURE_WRAP_S, mSpecs.WRAP_S);
    glTexParameteri(mSpecs.TARGET, GL_TEXTURE_WRAP_T, mSpecs.WRAP_T);
    glTexParameteri(mSpecs.TARGET, GL_TEXTURE_MIN_FILTER, mSpecs.MIN);
    glTexParameteri(mSpecs.TARGET, GL_TEXTURE_MAG_FILTER, mSpecs.MAG);
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

void Texture::LoadTexture2D()
{
    glTexImage2D(mSpecs.TARGET, 0, 
                 mSpecs.INTERNAL, 
                 mTexWidth, mTexHeight, 0, 
                 mSpecs.FORMAT, GL_FLOAT, NULL);
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