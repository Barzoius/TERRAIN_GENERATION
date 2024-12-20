#pragma once


#pragma once

#include "Drawable.h"
#include "Texture.hpp"

class Terrain : public Drawable
{
public:
    Terrain(float size);

    void SetPosition(glm::vec3 pos) noexcept;

    void SetRotation(float roll = 0.0f, float pitch = 0.0f, float yaw = 0.0f) noexcept;

    glm::mat4x4 GetTransformMatrix() const noexcept override;

    void ControlWND() noexcept;

    void SetHeightMap(std::unique_ptr<ShaderSuite>, int);

    ShaderSuite* GetComputeHeight();
    Texture* GetHeightMap();

private:
    glm::vec3 mPos = glm::vec3(0.0f, 0.0f, 0.0f);


    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;

    std::unique_ptr<ShaderSuite> computeHeightMap;
    std::unique_ptr<ShaderSuite> computeNormalMap;

    std::unique_ptr<Texture> heightMap;
    std::unique_ptr<Texture> normalMap;

    

};