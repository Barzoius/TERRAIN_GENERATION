#pragma once


#include <vector>
#include <memory>

#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders\Shader.h"
class Bindable;


class Drawable
{
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    ~Drawable() = default;

    void Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix) const noexcept;


    virtual glm::mat4x4 GetTransformMatrix() const noexcept = 0;

    ShaderSuite* GetShader() const noexcept;

protected:
    void AddBind(std::unique_ptr<Bindable>) noexcept;
    void AddElementBuffer(std::unique_ptr<class ElementBuffer>) noexcept;
    void AddShaderProgram(std::unique_ptr<ShaderSuite>) noexcept;

private:
    std::unique_ptr<class ShaderSuite> pShaderProgram;
    const class ElementBuffer* pElemBuffer = nullptr;

    std::vector<std::unique_ptr<Bindable>> bindables;
};