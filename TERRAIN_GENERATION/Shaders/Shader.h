#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include <vector>
#include <memory>



class Shader
{
public:
    enum class ShaderType : GLenum
    {
        VERTEX = GL_VERTEX_SHADER,
        TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER,
        TESELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        COMPUTE = GL_COMPUTE_SHADER,
    };

public:
    Shader(const std::string& sourceCode, ShaderType type);

    unsigned int GetID();

private:
    unsigned int ID;
};


class ShaderSuite
{
public:
    ShaderSuite(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>);

    Shader readShaderFromFile(std::string_view filePath, Shader::ShaderType);

    void use();

    void setBool(const std::string& uName, bool value) const;
    void setInt(const std::string& uName, int value) const;
    void setFloat(const std::string& uName, float value) const;
    void setVec2(const std::string& uName, float x, float y) const;
    void setVec2(const std::string& uName, const glm::vec2& vec) const;
    void setVec3(const std::string& uName, float x, float y, float z) const;
    void setVec3(const std::string& uName, const glm::vec3& vec) const;
    void setVec4(const std::string& uName, float x, float y, float z, float w) const;
    void setVec4(const std::string& uName, const glm::vec4& vec) const;
    void setMat2(const std::string& uName, const glm::mat2& mat) const;
    void setMat3(const std::string& uName, const glm::mat3& mat) const;
    void setMat4(const std::string& uName, const glm::mat4& mat) const;

    unsigned int GetID();

private:
    unsigned int ID;
};


