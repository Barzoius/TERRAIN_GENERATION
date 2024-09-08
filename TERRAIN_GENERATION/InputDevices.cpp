#include "InputDevices.h"
#include "Window.h"

namespace InputDevices
{

    Keyboard::Keyboard(const Window& wnd)
        :
        mWindow(std::make_unique<Window>(wnd.GetWidth(), wnd.GetHeight(), wnd.GetTitle()))
    {
        glfwSetKeyCallback(mWindow->GetWindow(), key_callback);
    };

    void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_W) // FORWARD
            {
                
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

            if (key == GLFW_KEY_Q) // UP
            {

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


    void Keyboard::EnableAutorepeat() noexcept
    {
        autoreapetOn = true;
    }

    void Keyboard::DisableAutorepeat() noexcept
    {
        autoreapetOn = false;
    }

    bool Keyboard::AutorepeatIsEnabled() const noexcept
    {
        return autoreapetOn;
    }

}


