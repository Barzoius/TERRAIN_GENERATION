#pragma once

#include "Bindable.h"


#include <string>
#include <optional>
#include <vector>

class Texture : public Bindable
{
public:

    enum class ETARGET : GLenum
    {
        TEX2D = GL_TEXTURE_2D,
        TEX2DARRAY = GL_TEXTURE_2D_ARRAY,
    };

    struct Specs
    {
        //ETARGET TARGET{ ETARGET::TEX2D };
        GLenum TARGET{ GL_TEXTURE_2D };
        GLint WRAP_S{ GL_CLAMP_TO_EDGE };
        GLint WRAP_T{ GL_CLAMP_TO_EDGE };
        GLint MIN{ GL_LINEAR };
        GLint MAG{ GL_LINEAR };
        GLint INTERNAL{ GL_RGB };
        GLenum FORMAT{ GL_RGB };
        bool MIPMAP{false};
        std::optional<GLsizei> layers{};
    };

    Texture(int width, int height, Specs sepcs);
    Texture();
    Texture(const Texture&) = delete;
    ~Texture();

    void Bind() override;
    void Unbind() override;

    void AccessBind(GLenum access);

    void createCubeMap(std::vector<const char*>);
    void loadFromFile(std::string_view);

    void LoadTexture2D();
    void LoadTexture2DFromFile(std::string_view);

    void LoadTexture2DArray(const std::vector<std::string_view>&);

    void SetUNIT(int unit);

    void SetActive();

    void SaveTexture();

    GLuint GetID();
    int GetWidth();
    int GetHeight();
    GLuint GetUnit();

    void SetParams();

    

private:
    int mTexWidth;
    int mTexHeight;

    GLuint texID;
    GLuint texUnit;
    static uint32_t nextUnit;


    Specs mSpecs{};

    bool isArray = false;

};