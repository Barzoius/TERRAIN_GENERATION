#include "Window.h"

#include <iostream>

static bool sGLFWInitialized = false;

Window::Window(int width, int height, const char* name)
{
    Init(width, height, name);
}

Window::~Window()
{
    ShutDown();
}



void Window::Init(int width, int height, const char* name)
{
    mData.Title = name;
    mData.Width = width;
    mData.Height = height;

    if (!sGLFWInitialized)
    {
        int success = glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

        sGLFWInitialized = true;
    }

    mWindow = glfwCreateWindow(mData.Width, mData.Height, mData.Title,
        nullptr, nullptr);

    //glfwSetWindowUserPointer(mWindow, this);

    glfwMakeContextCurrent(mWindow);

    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!///
    glfwSetFramebufferSizeCallback(mWindow,
        [](GLFWwindow* wnd, int width, int height)
        { glViewport(0, 0, width, height);  });

    //glfwSetKeyCallback(this->GetWindow(), key_callback);

    

}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_W) // FORWARD
        {
            //pWindow->mCamera.processKeyInput(Camera::CAM_MOVEMENT::FORWARD, 12.0f);
            std::cout << "W";
        }

        if (key == GLFW_KEY_S) // BACKWARD
        {

        }

        if (key == GLFW_KEY_A) // LEFT
        {

        }

        if (key == GLFW_KEY_D) // RIGHT
        {

        }

        if (key == GLFW_KEY_Q) // UPd
        {
            std::cout << "E";
        }

        if (key == GLFW_KEY_E) // DOWN
        {

        }
    }

    if (action == GLFW_RELEASE)
    {

    }

    if (action == GLFW_REPEAT)
    {

    }
}


///----SET THE CALLBACK AND DELETE THIS----///
void Window::ProcessInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mWindow, true);    
    }

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(mWindow, GLFW_KEY_O) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        mCamera.processKeyInput(Camera::FORWARD, deltaTime);
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        mCamera.processKeyInput(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        mCamera.processKeyInput(Camera::LEFT, deltaTime);
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        mCamera.processKeyInput(Camera::RIGHT, deltaTime);
    if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_PRESS)
        mCamera.processKeyInput(Camera::UP, deltaTime);
    if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
        mCamera.processKeyInput(Camera::DOWN, deltaTime);

}

const char* Window::GetTitle() const noexcept
{
    return mData.Title;
}

int Window::GetWidth() const noexcept
{
    return mData.Width;
}

int Window::GetHeight() const noexcept
{
    return mData.Height;
}

void Window::ShutDown()
{
    glfwDestroyWindow(mWindow);
}

void Window::OnUpdate()
{

    glfwPollEvents();
    glfwSwapBuffers(mWindow);
}


GLFWwindow* Window::GetWindow() const noexcept
{
    return mWindow;
}