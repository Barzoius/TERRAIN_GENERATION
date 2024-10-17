#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <memory>

#include "Camera.h"
//#include "InputDevices.h" 

class Window
{
public:
    Window(int width, int height, const char* name);
    ~Window();


    void Init(int width, int height, const char* name);
    void OnUpdate();
    void ShutDown();

    void ProcessInput();

    const char* GetTitle() const noexcept;
    int GetWidth() const noexcept;
    int GetHeight() const noexcept;

    GLFWwindow* GetWindow() const noexcept;
    Camera GetCmaera() const noexcept
    {
        return mCamera;
    }
private:
    static void key_callback(GLFWwindow* window,
        int keycode,
        int scancode,
        int action,
        int mods);

private:

    GLFWwindow* mWindow;

    struct SWindow_Mdata
    {
        const char* Title;
        int Width;
        int Height;
    };

    SWindow_Mdata mData;

public:

    Camera mCamera;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    //InputDevices::Keyboard keyboard;
    //InputDevices::Mouse mouse;
};