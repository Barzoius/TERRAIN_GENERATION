#include "Application.h"

int main()
{
    Application* app = new Application();
    app->Run();
    delete app;

    glfwTerminate();
    return 0;

}