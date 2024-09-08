#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <queue>
#include <bitset>
#include <memory>

class Window;

namespace InputDevices
{
   
    class Keyboard
    {
    public:
        
    public:
        Keyboard() = default;
        Keyboard(const Window&);

        void EnableAutorepeat() noexcept;
        void DisableAutorepeat() noexcept;
        bool AutorepeatIsEnabled() const noexcept;



    private:
        static void key_callback(GLFWwindow* window,
            int keycode,
            int scancode,
            int action,
            int mods);

        //static void character_callback(GLFWwindow* window, unsigned int codepoint);

    private:
        std::unique_ptr<Window> mWindow;

        bool autoreapetOn = true;

        static constexpr unsigned int nKeys = 265u;
        std::bitset<nKeys> keyStates;
    };


    //class Mouse
    //{
    //public:
    //    Mouse(GLFWwindow* window);


    //private:
    //    GLFWwindow* mWindow;

    //};



}
