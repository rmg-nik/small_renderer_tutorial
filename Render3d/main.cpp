#include <Render3d/Application.hpp>
#include <SDL.h>

int SDL_main(int argc, char** argv)
{
    render3d::Application app;
    return app.Run();
}
