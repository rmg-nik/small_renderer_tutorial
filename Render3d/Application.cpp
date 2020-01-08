#include <Render3d/Application.hpp>
#include <Render3d/Window.hpp>
#include <Render3d/Logic.hpp>
#include <SDL.h>

using namespace render3d;

class Application::Impl
{
public:

    int Run()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot init video '%s'", SDL_GetError());
            return -1;
        }
        Window window;
        if (window.Init(1280, 1024, true) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Cannot init window '%s'", SDL_GetError());
            return -1;
        }
        Logic logic(&window);

        SDL_Event ev;
        bool halt = false;
        while (!halt)
        {
            while (SDL_PollEvent(&ev) && !halt)
            {
                if (ev.type == SDL_QUIT)
                    halt = true;
            }

            window.Clear(Colors::Black);
            logic.Update(SDL_GetTicks());
            window.Flush();
            SDL_Delay(1);
        }

        return 0;
    }

private:


};

Application::Application()
{
    m_impl = std::make_shared<Application::Impl>();
}

int Application::Run()
{
    return m_impl->Run();
}
