#include "Shader.h"

void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


Shader::Shader(const std::string& sourceCode, ShaderType type)
    :
    ID(glCreateShader(static_cast<GLenum>(type)))
{
    const char* shaderCode = sourceCode.c_str();
    glShaderSource(ID, 1, &shaderCode, NULL);
    glCompileShader(ID);
    checkCompileErrors(ID, "---");
}

unsigned int Shader::GetID()
{
    return ID;
}

ShaderSuite::ShaderSuite(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>> suite)
    :
    ID(glCreateProgram())
{
    assert((suite.size() < 7) && "ERROR: suite contains more than 6 shaders.");

    std::vector<Shader> shaders;
    shaders.reserve(suite.size());
    for (const auto& p : suite)
    {
        const auto exits = mShaderLookUp.find(p.second);

        if (exits == mShaderLookUp.end())
        {   
            mShaderLookUp.insert({ p.second, true });
            shaders.emplace_back(readShaderFromFile(p.first, p.second));
            glAttachShader(ID, shaders.back().GetID());
        }
        else
        {
            std::cerr << "WARNINING: this shader program already has this type of shader\n It was not binded"<<std::endl;
        }


    }


    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
        
    for (auto& shader : shaders)
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


unsigned int ShaderSuite::GetID()
{
    return ID;
}

