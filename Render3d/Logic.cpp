#include <Render3d/Logic.hpp>
#include <Render3d/Window.hpp>
#include <cmath>

using namespace render3d;

namespace
{
    double DEG_TO_RAD(double deg)
    {
        return atan(1.0) * 4.0 * deg / 180.0;
    }
}

class Logic::Impl
{
public:

    Impl(Window* window)
    {
        m_window = window;
    }

    void Update(int curr_time)
    {
        int w = m_window->GetWidth();
        int h = m_window->GetHeight();
        for (int i = 0; i < w; ++i)
        {
            int x = i;
            int y = static_cast<int>((h / 4.0) * sin(curr_time / 500.0 + DEG_TO_RAD(i % 360))) + h / 2;
            m_window->DrawPixel(x, y, Colors::Green);
        }
    }

private:

    Window* m_window;
};

Logic::Logic(Window* window)
{
    m_impl = std::make_shared<Logic::Impl>(window);
}

void Logic::Update(int curr_time)
{
    m_impl->Update(curr_time);
}
