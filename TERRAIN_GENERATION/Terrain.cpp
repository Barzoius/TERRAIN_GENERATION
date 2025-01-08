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

ShaderSuite* Terrain::GetComputeNormal()
{
    return computeNormalMap.get();
}

Texture* Terrain::GetHeightMap()
{
    return heightMap.get();
}

Texture* Terrain::GetNormalMap()
{
    return normalMap.get();
}

void Terrain::SetHeightMap(std::unique_ptr<ShaderSuite> compute, float rez)
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

void Terrain::SetNormalMap(std::unique_ptr<ShaderSuite> compute, float rez)
{
    computeNormalMap = std::move(compute);

    const Texture::Specs specs{ .TARGET = GL_TEXTURE_2D,
                                .WRAP_S = GL_CLAMP_TO_EDGE,
                                .WRAP_T = GL_CLAMP_TO_EDGE,
                                .MIN = GL_LINEAR,
                                .MAG = GL_LINEAR,
                                .INTERNAL = GL_RGBA32F,
                                .FORMAT = GL_RGBA,
                                .MIPMAP = false };

    normalMap = std::make_unique<Texture>(rez, rez, specs);
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


    ///-----------------------------------ALBEDO-----------------------------------///

    albedos = std::make_unique<Texture>(2048, 2048, specs);

    albedos->SetUNIT(0);

    std::vector<std::string_view> albedoPATHS{
       "Resources/Materials/jagged-rockface1/JA_ALBEDO.png",
       "Resources/Materials/slate2-tiled/ST_ALBEDO.png",
       "Resources/Materials/rock-snow/RS_ALBEDO.png",
    };
    
    albedos->SetActive();
    albedos->Bind();
    albedos->LoadTexture2DArray(albedoPATHS);
    albedos->SetParams();
    //albedos->Unbind();

    //std::cout << albedos->GetUnit() << std::endl;
    ///-------------------------------------AO-------------------------------------///

    aos = std::make_unique<Texture>(2048, 2048, specs);

    aos->SetUNIT(3);

    std::vector<std::string_view> aoPATHS{

       "Resources/Materials/jagged-rockface1/JA_AO.png",
       "Resources/Materials/slate2-tiled/ST_AO.png",
       "Resources/Materials/rock-snow/RS_AO.png",
    };

    aos->SetActive();
    aos->Bind();
    aos->LoadTexture2DArray(aoPATHS);
    aos->SetParams();
    //aos->Unbind();

    //std::cout << aos->GetUnit() << std::endl;
    ///----------------------------------ROUGHNESS----------------------------------///

    roughness = std::make_unique<Texture>(2048, 2048, specs);

    roughness->SetUNIT(5);

    std::vector<std::string_view> roughnessPATHS{
       "Resources/Materials/jagged-rockface1/JA_ROUGH.png",
       "Resources/Materials/slate2-tiled/ST_ROUGH.png",
       "Resources/Materials/rock-snow/RS_ROUGH.png",
    };

    roughness->SetActive();
    roughness->Bind();
    roughness->LoadTexture2DArray(roughnessPATHS);
    roughness->SetParams();
    //roughness->Unbind();

    ///-----------------------------------NORMAL-----------------------------------///

    normals = std::make_unique<Texture>(2048, 2048, specs);

    normals->SetUNIT(4);

    std::vector<std::string_view> normalPATHS{
       "Resources/Materials/jagged-rockface1/JA_NORMAL.png",
       "Resources/Materials/slate2-tiled/ST_NORMAL.png",
       "Resources/Materials/rock-snow/RS_NORMAL.png",
    };

    normals->SetActive();
    normals->Bind();
    normals->LoadTexture2DArray(normalPATHS);
    normals->SetParams();
    //normals->Unbind();


    ///----------------------------------METALLIC----------------------------------///

    heights = std::make_unique<Texture>(2048, 2048, specs);

    heights->SetUNIT(6);

    std::vector<std::string_view> heightPATHS{
       "Resources/Materials/jagged-rockface1/JA_METALLIC.png", 
       "Resources/Materials/slate2-tiled/ST_METALLIC.png",
       "Resources/Materials/rock-snow/RS_METALLIC.png",
    };

    heights->SetActive();
    heights->Bind();
    heights->LoadTexture2DArray(heightPATHS);
    heights->SetParams();
    //heights->Unbind();

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
        ImGui::Image((void*)(intptr_t)this->GetHeightMap()->GetID(), ImVec2(150, 150));

        ImGui::SliderInt("Algorithm", &Alg, 0, 2);

        ImGui::SliderInt("SEED", &seed, 1, 16);
        
        ImGui::Text("FBM");
        ImGui::SliderInt("OCTAVES", &octaves, 1, 16);
        ImGui::SliderFloat("Lacunarity", &lacunarity, 0.0f, 8.0f);
        ImGui::SliderFloat("Persistance", &persistance, 0.0f, 8.0f);
        ImGui::SliderFloat("Scale", &noise_scale, 0.0f, 20.0f);
        ImGui::SliderFloat("Exponent", &exponent, 0.0f, 5.0f);

        ImGui::Text("FF");
        ImGui::SliderInt("Iterations", &iterations, 16, 100);
       

        //ImGui::Text("DS");
        //ImGui::SliderFloat("RoughFactor", &roughFactor, 1.0, 8.0);

        ImGui::Text("NormalMap");
        ImGui::Image((void*)(intptr_t)this->GetNormalMap()->GetID(), ImVec2(150, 150));


        ImGui::Text("Texture Options");

        ImGui::Checkbox("TriplanarTexturing", &triplanar);

        ImGui::Checkbox("Texturing Bombing", &bombIT);

        ImGui::Checkbox("PBR", &PBR);

    }
    ImGui::End();

    //std::cout << GetHeightMap()->GetUnit() << " " << GetNormalMap()->GetUnit()<<" ";

  
}