#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "Application.h"
#include "iostream"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ElementBuffer.h"


#include <algorithm>

Application::Application()
{
    mWindow = std::make_unique<Window>(800, 600, "TOY_GFX");



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD FAILED";
    }

    light = new LightSource(2.0f);
    light->SetPosition(glm::vec3(1.5f, 3.0f, 0.0f));


    terrain = new Terrain(20.0f);
    terrain->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    terrain->SetRotation(35.0f, 35.0f, 35.0f);


    SkyBox = std::make_unique<Texture>();


    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

Application::~Application()
{

}

void Application::OnEvent()
{
    if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mWindow->GetWindow(), true);
    }

    if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_P) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
    {
        //glfwSetWindowShouldClose(mWindow->GetWindow(), true);
        std::cout << "E";
    }

}



void Application::Run()
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mWindow->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");


    SkyBoxShader = std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shaders/SkyBoxVert.glsl", Shader::ShaderType::VERTEX},
        { "Shaders/SkyBoxFrag.glsl", Shader::ShaderType::FRAGMENT }});

    
    
    SkyBox->createCubeMap({"Resources/SkyBox/EpicBlueSunset/-X.png",
                           "Resources/SkyBox/EpicBlueSunset/+X.png",
                           "Resources/SkyBox/EpicBlueSunset/-Y.png",
                           "Resources/SkyBox/EpicBlueSunset/+Y.png",
                           "Resources/SkyBox/EpicBlueSunset/-Z.png",
                           "Resources/SkyBox/EpicBlueSunset/+Z.png"});


    int width = mWindow->GetWidth();
    int height = mWindow->GetHeight();

    width = width;
    height = height * 2;

    terrain->SetHeightMap(std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shaders/HeightMap.glsl", Shader::ShaderType::COMPUTE},}), width);

    terrain->SetNormalMap(std::make_unique<ShaderSuite>(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shaders/NormalMap.glsl", Shader::ShaderType::COMPUTE}, }), width);

    terrain->GetHeightMap()->SetActive();
    terrain->GetHeightMap()->Bind();
    terrain->GetHeightMap()->LoadTexture2D();
    terrain->GetHeightMap()->SetParams();
    terrain->GetHeightMap()->AccessBind(GL_READ_WRITE);
    terrain->GetHeightMap()->Unbind();


    terrain->GetNormalMap()->SetActive();
    terrain->GetNormalMap()->Bind();
    terrain->GetNormalMap()->LoadTexture2D();
    terrain->GetNormalMap()->SetParams();
    terrain->GetNormalMap()->AccessBind(GL_READ_WRITE);
    terrain->GetNormalMap()->Unbind();


    terrain->SetMaterialData(width);


    glm::mat4 cameraView;
    glm::mat4 projection = glm::mat4(1.0f);

    GLint maxTessLevel;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);
    std::cout << "Max available tess level: " << maxTessLevel << std::endl;


    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Since the vertex data is hardcoded in the shader, no VBO setup is needed here.
    // Just ensure the VAO is created and bound.

    glBindVertexArray(0);
 
    while (!glfwWindowShouldClose(mWindow->GetWindow()))
    {
       
        mWindow->ProcessInput();

      
        //glClearColor(0.91f, 0.64f, 0.09f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cameraView = mWindow->mCamera.GetMatrix();
        cameraView = glm::translate(cameraView, glm::vec3(0.0f, 0.0f, 1.0f));

        GLint error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }

        projection = glm::perspective(glm::radians(45.0f), (float)mWindow->GetWidth() / (float)mWindow->GetHeight(), 0.1f, 100.0f);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        SkyBoxShader->use();
        SkyBoxShader->setMat4("view", glm::mat4(glm::mat3(cameraView)));
        SkyBoxShader->setMat4("projection", projection);
        glDepthMask(GL_FALSE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox->GetID());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);

        terrain->GetHeightMap()->Bind();

        terrain->GetComputeHeight()->use();
        terrain->GetComputeHeight()->setInt("iterations", terrain->iterations);
        terrain->GetComputeHeight()->setVec2("resolution", (float)width, (float)width);
        terrain->GetComputeHeight()->setInt("octaves", terrain->octaves);
        terrain->GetComputeHeight()->setFloat("lacunarity", terrain->lacunarity);
        terrain->GetComputeHeight()->setFloat("persistence", terrain->persistance);
        terrain->GetComputeHeight()->setFloat("scale", terrain->noise_scale);
        terrain->GetComputeHeight()->setFloat("exponent", terrain->exponent);



        glDispatchCompute((width) / 16, (width) / 16, 1); 
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, terrain->GetHeightMap()->GetID());


        terrain->GetComputeNormal()->use();
        terrain->GetComputeNormal()->setInt("operator", 2);
    

        glDispatchCompute((width) / 16, (width) / 16, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);


        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, terrain->GetNormalMap()->GetID());


        if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_C) == GLFW_PRESS)
        {
            terrain->GetHeightMap()->SaveTexture();
            //terrain->GetNormalMap()->SaveTexture();
        }


        terrain->GetShader()->use();
        terrain->GetShader()->setInt("heightMap", 1);
        terrain->GetShader()->setInt("normalMap", 2);

        terrain->GetShader()->setVec3("camOrigin", mWindow->mCamera.GetPosition());
        terrain->GetShader()->setVec3("lightOrigin", light->GetPosition());

         terrain->GetShader()->setInt("ALBEDO", 0);
         terrain->GetShader()->setInt("AO", 3);

         terrain->GetShader()->setBool("triplanar", terrain->triplanar);


     
      


        light->ControlWND();
        light->DrawIndexed(cameraView, projection);

        terrain->ControlWND();
        terrain->Draw(cameraView, projection);



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        mWindow->OnUpdate();
    }
}


bool Application::OnWindowClose()
{
    mIsRunning = false;
    return true;
}
