#include "Application.h"

#include <fstream>
#include <string>
#include <iostream>

int main()
{
    //std::cout << __cplusplus;

    Application* app = new Application();
    app->Run();
    delete app;

    glfwTerminate();
    return 0;

}