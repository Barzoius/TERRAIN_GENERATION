#include "Shader.h"

#include <type_traits>


Shader::Shader(const std::string& sourceCode, ShaderType type)
    :
    ID(static_cast<GLenum>(type))
{
    const char* shaderCode = sourceCode.c_str();
    glShaderSource(ID, 1, &shaderCode, NULL);
    glCompileShader(ID);
}

unsigned int Shader::GetID()
{
    return ID;
}

ShaderSuite::ShaderSuite(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>> suite)
    :
    ID(glCreateProgram())
{
    for (const auto& p : suite)
    {
        Shader shader = readShaderFromFile(p.first, p.second);
        mShaders.emplace_back(shader);
        glAttachShader(ID, shader.GetID());
    }

    glLinkProgram(ID);

    for (auto& shader : mShaders)
    {
        glDetachShader(ID, shader.GetID());
        glDeleteShader(shader.GetID());
    }
}



Shader ShaderSuite::readShaderFromFile(std::string_view filePath, Shader::ShaderType type)
{
    std::string shaderCode;

    std::ifstream shaderSource;
    shaderSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderSource.open(filePath.data());

        std::stringstream shaderStream;
        shaderStream << shaderSource.rdbuf();

        shaderSource.close();
        
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }


    return Shader(shaderCode, type);
}

void ShaderSuite::use()
{
    glUseProgram(ID);
}


void ShaderSuite::setBool(const std::string& uName, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, uName.c_str()), (int)value);
}

void ShaderSuite::setInt(const std::string& uName, int value) const
{
    glUniform1i(glGetUniformLocation(ID, uName.c_str()), value);
}

void ShaderSuite::setFloat(const std::string& uName, float value) const
{
    glUniform1f(glGetUniformLocation(ID, uName.c_str()), value);
}

void ShaderSuite::setVec2(const std::string& uName, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, uName.c_str()), x, y);
}

void ShaderSuite::setVec2(const std::string& uName, const glm::vec2& vec) const
{
    glUniform2fv(glGetUniformLocation(ID, uName.c_str()), 1, &vec[0]);
}

void ShaderSuite::setVec3(const std::string& uName, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, uName.c_str()), x, y, z);
}

void ShaderSuite::setVec3(const std::string& uName, const glm::vec3& vec) const
{
    glUniform3fv(glGetUniformLocation(ID, uName.c_str()), 1, &vec[0]);
}

void ShaderSuite::setVec4(const std::string& uName, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, uName.c_str()), x, y, z, w);
}

void ShaderSuite::setVec4(const std::string& uName, const glm::vec4& vec) const
{
    glUniform4fv(glGetUniformLocation(ID, uName.c_str()), 1, &vec[0]);
}

void ShaderSuite::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderSuite::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderSuite::setMat4(const std::string& uName, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, uName.c_str()), 1, GL_FALSE, &mat[0][0]);
}
