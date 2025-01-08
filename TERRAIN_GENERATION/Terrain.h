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

    void SetHeightMap(std::unique_ptr<ShaderSuite>, float);
    void SetNormalMap(std::unique_ptr<ShaderSuite>, float);

    void SetMaterialData(int);


    ShaderSuite* GetComputeHeight();
    ShaderSuite* GetComputeNormal();

    Texture* GetHeightMap();
    Texture* GetNormalMap();

private:
    glm::vec3 mPos = glm::vec3(0.0f, 0.0f, 0.0f);


    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;

    std::unique_ptr<ShaderSuite> computeHeightMap;
    std::unique_ptr<ShaderSuite> computeNormalMap;

    std::unique_ptr<Texture> heightMap;
    std::unique_ptr<Texture> normalMap;

    std::unique_ptr<Texture> albedos;
    std::unique_ptr<Texture> aos;
    std::unique_ptr<Texture> roughness;
    std::unique_ptr<Texture> normals;
    std::unique_ptr<Texture> heights;


public:

    int seed = 1.0;


        int octaves = 1.0;
        float lacunarity = 1.0;
        float persistance = 1.0;
        float noise_scale = 1.0;
        float exponent = 1.0;



        int stepSize = 256;
        float scale = 0.5;
        float roughFactor = 1;
   


    int iterations;

    int Alg = 2;
    
   
    float rooughWeight = 1.0;

    bool bombIT = false;
    bool triplanar;
    bool PBR = false;
};