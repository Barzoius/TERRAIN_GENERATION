#include "Application.h"
#include "iostream"

Application::Application()
{
    mWindow = std::make_unique<Window>(800, 600 , "TERRAIN_GENERATION");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD FAILED";
    }
}

Application::~Application()
{

}


void Application::Run()
{

    while (!glfwWindowShouldClose(mWindow->GetWindow()))
    {

        mWindow->ProcessInput();

        glClearColor(0.91f, 0.64f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mWindow->OnUpdate();
    }
}

bool Application::OnWindowClose()
{
    mIsRunning = false;
    return true;
}