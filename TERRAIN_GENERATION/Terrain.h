#pragma once


#pragma once

#include "Drawable.h"

class Terrain : public Drawable
{
public:
    Terrain(float size);

    void SetPosition(glm::vec3 pos) noexcept;

    void SetRotation(float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f) noexcept;

    glm::mat4x4 GetTransformMatrix() const noexcept override;

    void ControlWND() noexcept;


private:
    glm::vec3 mPos = glm::vec3(0.0f, 0.0f, 0.0f);


    float roll = -240.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;


};