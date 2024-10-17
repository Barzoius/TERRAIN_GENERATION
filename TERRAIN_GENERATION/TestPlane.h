#pragma once

#include "Drawable.h"
#include "ElementBuffer.h"

class TestPlane : public Drawable
{
public:
    TestPlane(float size);

    void SetPosition(glm::vec3 pos) noexcept;

    void SetRotation(float roll, float pitch, float yaw) noexcept;

    glm::mat4x4 GetTransformMatrix() const noexcept override;

    void ControlWND() noexcept;


private:
    glm::vec3 mPos = glm::vec3(0.0f, 0.0f, 0.0f);


    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;

};