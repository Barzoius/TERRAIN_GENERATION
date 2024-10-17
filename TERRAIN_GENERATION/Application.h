#include "Window.h"
#include "Camera.h"
#include "Timer.h"
#include "Shaders\Shader.h"

#include "memory"


#include "LightSource.h"
#include "TestPlane.h"




class Application
{
public:
    Application();
    ~Application();

    void Run();
    void OnEvent();

    bool OnWindowClose();

    inline Window* GetWindow() { return mWindow.get(); }

 
    //LightSource* light;
    TestPlane* plane;

private:
    std::unique_ptr<Window> mWindow;
    bool mIsRunning = true;

    Camera camera;
    Timer timer;

    // Projection proj;




    std::unique_ptr<ShaderSuite> shaderProgram{};
};