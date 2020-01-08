#include <Render3d/Window.hpp>
#include <SDL.h>

using namespace render3d;

const Color Colors::White   = { 255, 255, 255 };
const Color Colors::Black   = {   0,   0,   0 };
const Color Colors::Red     = { 255,   0,   0 };
const Color Colors::Green   = {   0, 255,   0 };
const Color Colors::Blue    = {   0,   0, 255 };

class Window::Impl
{
public:

    Impl() = default;

    ~Impl()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    int Init(int w, int h, bool flip_y)
    {
        m_flip_y = flip_y;

        SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
        m_window = SDL_CreateWindow("Render3d App",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w, h,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!m_window)
            return -1;

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_renderer)
            return -1;

        return 0;
    }

    void Clear(const Color& color)
    {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_renderer);
    }

    void Flush()
    {
        SDL_RenderPresent(m_renderer);
    }

    void DrawPixel(int x, int y, const Color& color)
    {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(m_renderer, x, m_flip_y ? (GetHeight() - y) : y);
    }

    int GetWidth() const
    {
        int w;
        SDL_GetWindowSize(m_window, &w, nullptr);
        return w;
    }

    int GetHeight() const
    {
        int h;
        SDL_GetWindowSize(m_window, nullptr, &h);
        return h;
    }
private:

    SDL_Window*     m_window{ nullptr };
    SDL_Renderer*   m_renderer{ nullptr };
    bool m_flip_y{ false };

};

render3d::Window::Window() = default;

int render3d::Window::Init(int width, int height, bool flip_y)
{
    if (m_impl)
        return 1;

    m_impl = std::make_shared<Window::Impl>();
    auto ret = m_impl->Init(width, height, flip_y);
    if (ret < 0)
        m_impl.reset();

    return ret;
}

void Window::Clear(const Color& color)
{
    m_impl->Clear(color);
}

void Window::Flush()
{
    m_impl->Flush();
}

void render3d::Window::DrawPixel(int x, int y, const Color& color)
{
    m_impl->DrawPixel(x, y, color);
}

int Window::GetWidth() const
{
    return m_impl->GetWidth();
}

int Window::GetHeight() const
{
    return m_impl->GetHeight();
}
