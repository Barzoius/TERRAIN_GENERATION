//#include "imgui\imgui.h"
//#include "imgui\imgui_impl_glfw.h"
//#include "imgui\imgui_impl_opengl3.h"

#include "Application.h"
#include "iostream"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ElementBuffer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <algorithm>

Application::Application()
{
    mWindow = std::make_unique<Window>(800, 600, "TOY_GFX");



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD FAILED";
    }

    //light = new LightSource(1.0f);
    //light->SetPosition(glm::vec3(1.5f, 3.0f, 0.0f));

    //plane = new TestPlane(1.0f);
    //plane->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    //plane->SetRotation(35.0f);

    terrain = new Terrain(20.0f);
    terrain->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    terrain->SetRotation(90.0f);

    glEnable(GL_DEPTH_TEST);
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
    // Load the compute shader
    ShaderSuite compute = ShaderSuite(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shaders/HeightMap.glsl", Shader::ShaderType::COMPUTE},
    });

    // Get window dimensions
    int width = mWindow->GetWidth();
    int height = mWindow->GetHeight();

    width = width * 2;
    height = height * 2;
    // Generate and configure the texture
    unsigned int texture;
    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture);

    // Allocate storage for the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Bind the texture to an image unit for the compute shader
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);


    glm::mat4 cameraView;
    glm::mat4 projection = glm::mat4(1.0f);

    GLint maxTessLevel;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);
    std::cout << "Max available tess level: " << maxTessLevel << std::endl;

 
    while (!glfwWindowShouldClose(mWindow->GetWindow()))
    {
       
        mWindow->ProcessInput();

      
        glClearColor(0.91f, 0.64f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        compute.use();
        compute.setInt("iterations", 64);
        compute.setVec2("resolution", width, height);

        // Bind the texture for compute shader writes
        glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        glDispatchCompute((width + 15) / 16, (height + 15) / 16, 1); // Ensure proper group sizes

        // Ensure all writes to the texture are completed before use
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);

        // Bind the texture for the tessellation shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        terrain->GetShader()->use();
        terrain->GetShader()->setInt("heightMap", 0);

     
        if (glfwGetKey(mWindow->GetWindow(), GLFW_KEY_C) == GLFW_PRESS)
        {
  
            std::vector<float> textureData(width * height * 4);
            glBindTexture(GL_TEXTURE_2D, texture);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, textureData.data());

            std::cout << "Texture Data (first 10 pixels):\n";
            for (size_t i = 0; i < 10; ++i)
            {
                std::cout << "Pixel " << i << ": ("
                    << textureData[i * 4 + 0] << ", "
                    << textureData[i * 4 + 1] << ", "
                    << textureData[i * 4 + 2] << ", "
                    << textureData[i * 4 + 3] << ")\n";
            }

     
            std::vector<unsigned char> imageData(width * height * 4);
            for (size_t i = 0; i < textureData.size(); ++i)
            {
                imageData[i] = static_cast<unsigned char>(std::clamp(textureData[i] * 255.0f, 0.0f, 255.0f));
            }

            stbi_write_png("output_texture.png", width, height, 4, imageData.data(), width * 4);
            std::cout << "Saved texture to output_texture.png\n";
        }


        cameraView = mWindow->mCamera.GetMatrix();
        cameraView = glm::translate(cameraView, glm::vec3(0.0f, 0.0f, 1.0f));


        projection = glm::perspective(glm::radians(45.0f), (float)mWindow->GetWidth() / (float)mWindow->GetHeight(), 0.1f, 100.0f);

        terrain->ControlWND();
        terrain->Draw(cameraView, projection);

        mWindow->OnUpdate();
    }
}


bool Application::OnWindowClose()
{
    mIsRunning = false;
    return true;
}
