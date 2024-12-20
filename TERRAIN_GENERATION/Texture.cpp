#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <vector>

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
    glBindTexture(mSpecs.TARGET, 0);
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
    //glCreateTextures(mSpecs.TARGET, 1, &texID);
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


void Texture::LoadTexture2DArray(const std::vector<std::string_view>& filePaths)
{
    stbi_set_flip_vertically_on_load(true);

   

    int width = 0, height = 0;

    int numChannels = 0;
    
    std::vector<unsigned char*> imageData(filePaths.size());

    for (GLint i = 0; i < static_cast<GLint>(filePaths.size()); i++)
    {
       
 
        imageData[i] = stbi_load(filePaths[i].data(), &width, &height, &numChannels, 4);

        if (!imageData[i]) {
            std::cerr << "Failed to load image: " << filePaths[i] << std::endl;
            // Free previously loaded images
            for (size_t j = 0; j < i; ++j) {
                stbi_image_free(imageData[j]);
            }
            return;
        }
      
       
    }

    glTexStorage3D(mSpecs.TARGET, 1, GL_RGBA8, width, height, filePaths.size());
    for (size_t i = 0; i < filePaths.size(); ++i) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData[i]);
        stbi_image_free(imageData[i]); 
    }



}


void Texture::SetUNIT(int u)
{
    texUnit = u;
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