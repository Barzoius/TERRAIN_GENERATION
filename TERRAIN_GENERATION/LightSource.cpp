#include "LightSource.h"
#include "Sphere.h"
#include "BindableObjects.h"
#include "Shaders\Shader.h"

#include <string>

//#include "imgui/imgui.h"

LightSource::LightSource(float size)
{
    struct VERTEX
    {
        glm::vec3 pos;
    };

    auto model = Sphere::Make<VERTEX>();

    model.Treansform(glm::vec3(size * 0.5f, size * 0.5f, size * 0.5f));

    AddBind(std::make_unique<VertexArray>());

    AddBind(std::make_unique<VertexBuffer<VERTEX>>(model.vertices));           // vertices


    AddElementBuffer(std::make_unique<ElementBuffer>(model.indices));  // indices

    AddShaderProgram(std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shaders/VertShader.glsl", Shader::ShaderType::VERTEX},
        { "Shaders/FragShader.glsl", Shader::ShaderType::FRAGMENT },
    }));

}

void LightSource::SetPosition(glm::vec3 pos) noexcept
{
    mPos.x = pos.x;
    mPos.y = pos.y;
    mPos.z = pos.z;

}

glm::mat4x4 LightSource::GetTransformMatrix() const noexcept
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 0.0f, 1.0f)) *    // Yaw (Z-axis)
        glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(0.0f, 1.0f, 0.0f)) *  // Pitch (Y-axis)
        glm::rotate(glm::mat4(1.0f), roll, glm::vec3(1.0f, 0.0f, 0.0f));    // Roll (X-axis)

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPos);

    return translation * rotation;
}



void LightSource::ControlWND() noexcept
{
    //if (ImGui::Begin("Light"))
    //{
    //    ImGui::Text("Position");
    //    ImGui::SliderFloat("X", &mPos.x, -80.0f, 80.0f, "%.1f");
    //    ImGui::SliderFloat("Y", &mPos.y, -80.0f, 80.0f, "%.1f");
    //    ImGui::SliderFloat("Z", &mPos.z, -80.0f, 80.0f, "%.1f");

    //}
    //ImGui::End();
}