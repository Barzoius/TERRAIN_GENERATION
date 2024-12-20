#include "Terrain.h"

#include "TerrainPlane.h"
#include "BindableObjects.h"
#include "Shaders\Shader.h"

#include <string>


#include "imgui/imgui.h"

Terrain::Terrain(float size)
{
    struct VERTEX
    {
        glm::vec3 pos;
        glm::vec2 tex;
    };

    auto model = TerrainPlane::Make<VERTEX>();

    model.Treansform(glm::vec3(size, size, size));

    AddBind(std::make_unique<VertexArray>());

    AddBind(std::make_unique<VertexBuffer<VERTEX>>(model.vertices));           // vertices


    AddShaderProgram(std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shaders/TerrainVert.glsl", Shader::ShaderType::VERTEX},
        {"Shaders/TerrainFrag.glsl", Shader::ShaderType::FRAGMENT},
        {"Shaders/TCS.glsl", Shader::ShaderType::TESSELLATION_CONTROL},
        { "Shaders/TES.glsl", Shader::ShaderType::TESELLATION_EVALUATION}
    }));



}

ShaderSuite* Terrain::GetComputeHeight()
{
    return computeHeightMap.get();
}

Texture* Terrain::GetHeightMap()
{
    return heightMap.get();
}

void Terrain::SetHeightMap(std::unique_ptr<ShaderSuite> compute, int rez)
{
    computeHeightMap = std::move(compute);

    const Texture::Specs specs{ .TARGET = GL_TEXTURE_2D,
                                .WRAP_S = GL_CLAMP_TO_EDGE,
                                .WRAP_T = GL_CLAMP_TO_EDGE,
                                .MIN = GL_LINEAR,
                                .MAG = GL_LINEAR,
                                .INTERNAL = GL_RGBA32F,
                                .FORMAT = GL_RGBA,
                                .MIPMAP = false};

    heightMap = std::make_unique<Texture>(rez, rez, specs);
}

void Terrain::SetMaterialData(int rez)
{
    const Texture::Specs specs{ .TARGET = GL_TEXTURE_2D_ARRAY,
                                .WRAP_S = GL_REPEAT,
                                .WRAP_T = GL_REPEAT,
                                .MIN = GL_LINEAR_MIPMAP_LINEAR,
                                .MAG = GL_LINEAR,
                                .INTERNAL = GL_RGBA32F,
                                .FORMAT = GL_RGBA,
                                .MIPMAP = true,
                                .layers = 3};

    albedos = std::make_unique<Texture>(2048, 2048, specs);

    albedos->SetUNIT(0);

    std::vector<std::string_view> albedoPATHS{
       "Resources/Materials/rocky-dirt/RD_ALBEDO.png",
       "Resources/Materials/jagged-rockface1/JA_ALBEDO.png",
       "Resources/Materials/rock-snow/RS_ALBEDO.png",
    };
    
    albedos->SetActive();
    albedos->Bind();
    albedos->LoadTexture2DArray(albedoPATHS);
    albedos->SetParams();
    //albedos->Unbind();

}





void Terrain::SetPosition(glm::vec3 pos) noexcept
{
    mPos.x = pos.x;
    mPos.y = pos.y;
    mPos.z = pos.z;

}

void Terrain::SetRotation(float roll, float pitch, float yaw) noexcept
{
    roll = roll;
    pitch = pitch;
    yaw = yaw;
}

glm::mat4x4 Terrain::GetTransformMatrix() const noexcept
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 0.0f, 1.0f)) *    // Yaw (Z-axis)
        glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(0.0f, 1.0f, 0.0f)) *  // Pitch (Y-axis)
        glm::rotate(glm::mat4(1.0f), roll, glm::vec3(1.0f, 0.0f, 0.0f));    // Roll (X-axis)

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPos);

    return translation * rotation;
}



void Terrain::ControlWND() noexcept
{
    if (ImGui::Begin("TERRAIN"))
    {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &mPos.x, -80.0f, 80.0f, "%.1f");
        ImGui::SliderFloat("Y", &mPos.y, -80.0f, 80.0f, "%.1f");
        ImGui::SliderFloat("Z", &mPos.z, -80.0f, 80.0f, "%.1f");
        ImGui::Text("Orientation");
        ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
        ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

        ImGui::Text("HeightMap");
        ImGui::Image((void*)(intptr_t)this->GetHeightMap()->GetID() , ImVec2(150, 150));

    }
    ImGui::End();
}