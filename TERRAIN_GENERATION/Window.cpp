#include "Window.h"
#include "InputDevices.h"

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

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

}

void Window::ProcessInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mWindow, true);    
    }
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