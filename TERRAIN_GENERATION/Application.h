#include "Window.h"
#include "Camera.h"
#include "Timer.h"

#include "memory"

class Application
{
public:
    Application();
    ~Application();

    void Run();
    void OnEvent();

    bool OnWindowClose();

    inline Window* GetWindow() { return mWindow.get(); }


private:
    std::unique_ptr<Window> mWindow;
    bool mIsRunning = true;

    Camera camera;
    Timer timer;
};