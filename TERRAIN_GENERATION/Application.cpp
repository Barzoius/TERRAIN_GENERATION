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





Application::Application()
{
    mWindow = std::make_unique<Window>(800, 600, "TOY_GFX");



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD FAILED";
    }

    //light = new LightSource(1.0f);
    //light->SetPosition(glm::vec3(1.5f, 3.0f, 0.0f));

    plane = new TestPlane(1.0f);
    plane->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    plane->SetRotation(35.0f);


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


void SaveHeightMap(GLuint texture, int width, int height, const std::string& filename)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    // Allocate buffer to hold texture data
    std::vector<float> pixels(width * height * 4); // Assuming GL_RGBA32F



    // Read the texture data
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels.data());

    for (int i = 0; i < 10; ++i) {
        std::cout << "Pixel " << i << ": R=" << pixels[i * 4 + 0]
            << ", G=" << pixels[i * 4 + 1]
            << ", B=" << pixels[i * 4 + 2]
            << ", A=" << pixels[i * 4 + 3] << std::endl;
    }

    // Convert to 8-bit format for saving
    std::vector<unsigned char> image(width * height * 4);
    for (int i = 0; i < width * height * 4; ++i)
    {
        // Scale the float range (0.0f to 1.0f) to 8-bit (0 to 255)
        image[i] = static_cast<unsigned char>(glm::clamp(pixels[i] * 255.0f, 0.0f, 255.0f));
    }

    // Save the image as BMP
    if (!stbi_write_bmp(filename.c_str(), width, height, 4, image.data())) {
        std::cerr << "Failed to write BMP file!" << std::endl;
    }

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::Run()
{
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImGui::StyleColorsDark();
    //ImGui_ImplGlfw_InitForOpenGL(mWindow->GetWindow(), true);
    //ImGui_ImplOpenGL3_Init("#version 330");


    ShaderSuite compute = ShaderSuite(std::initializer_list<std::pair<std::string_view, Shader::ShaderType>>{
        {"Shaders/HeightMap.glsl", Shader::ShaderType::COMPUTE},
    });

    int width = mWindow->GetWidth();
    int height = mWindow->GetHeight();

    GLuint heightmapTexture;
    glGenTextures(1, &heightmapTexture);
    glBindTexture(GL_TEXTURE_2D, heightmapTexture);




    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, heightmapTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);


    GLuint texture;


    glm::mat4 cameraView;
    glm::mat4 projection = glm::mat4(1.0f);

    int Divs = 8;

    float squareSize = 2.0f / Divs;


    Texture tex(0, 0);
    tex.Bind();
    tex.SetParams();
    tex.loadFromFile("Resources/HeightMaps/HMAP_2.png");



    while (!glfwWindowShouldClose(mWindow->GetWindow()))
    {

        mWindow->ProcessInput();


        glClearColor(0.91f, 0.64f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();




        compute.use();
         
        compute.setInt("iterations", 32);
        compute.setVec2("resolution", width, height);
        glDispatchCompute(width, height, 1);
        //glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT)
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);



        cameraView = mWindow->mCamera.GetMatrix();
        cameraView = glm::translate(cameraView, glm::vec3(0.0f, 0.0f, 1.0f));

        projection = glm::perspective(glm::radians(45.0f), (float)mWindow->GetWidth() / (float)mWindow->GetHeight(), 0.1f, 100.0f);




        /*light->ControlWND();
        light->Draw(cameraView, projection);
        light->GetShader()->setVec3("Color", 1.0f, 1.0f, 1.0f);*/

        plane->ControlWND();
        plane->Draw(cameraView, projection);
        plane->GetShader()->setVec3("Color", 1.0f, 1.0f, 1.0f);



        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        mWindow->OnUpdate();
    }

   // SaveHeightMap(heightmapTexture, width, height, "Shaders/HeightMap.bmp");


}

bool Application::OnWindowClose()
{
    mIsRunning = false;
    return true;
}
