#include <Render3d/Logic.hpp>
#include <Render3d/Window.hpp>
#include <Render3d/tinyrenderer/model.h>
#include <cmath>

using namespace render3d;

namespace
{
    double DEG_TO_RAD(double deg)
    {
        return atan(1.0) * 4.0 * deg / 180.0;
    }

    void draw_line(int x1, int y1, int x2, int y2, const Color& color, Window& window)
    {
        bool transposed = false;
        if (abs(x2 - x1) < abs(y2 - y1))
        {
            std::swap(x1, y1);
            std::swap(x2, y2);
            transposed = true;
        }
        if (x2 < x1)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const int dx = x2 - x1;
        const int dy = y2 - y1;
        const int derror2 = abs(dy) * 2;
        int error2 = 0;
        int x = x1;
        int y = y1;
        for (; x <= x2; ++x)
        {
            transposed ? window.DrawPixel(y, x, color) : window.DrawPixel(x, y, color);
            error2 += derror2;
            if (error2 > dx)
            {
                error2 -= 2 * dx;
                y += (y2 > y1) ? 1 : -1;
            }
        }
    }
}

class Logic::Impl
{
public:

    Impl(Window* window) : m_model("african_head.obj")
    {
        m_window = window;
    }

    void Update(int curr_time)
    {
        int w = m_window->GetWidth();
        int h = m_window->GetHeight();
        /*
        int prev_x = -1;
        int prev_y = -1;
        for (int i = 0; i < w; ++i)
        {
            int x = i;
            int y = static_cast<int>((h / 4.0) * sin(curr_time / 500.0 + DEG_TO_RAD(i % 360))) + h / 2;
            if (prev_x < 0)
            {
                prev_x = x;
                prev_y = y;
            }
            draw_line(x, y, prev_x, prev_y, Colors::Green, *m_window);
            //m_window->DrawPixel(x, y, Colors::Blue);
            prev_x = x;
            prev_y = y;
        }
        */

        for (int i = 0; i < m_model.nfaces(); i++)
        {
            std::vector<int> face = m_model.face(i);
            for (int j = 0; j < 3; j++)
            {
                Vec3f v0 = m_model.vert(face[j]);
                Vec3f v1 = m_model.vert(face[(j + 1) % 3]);
                int x0 = static_cast<int>((v0.x + 1.) * w / 2.);
                int y0 = static_cast<int>((v0.y + 1.) * h / 2.);
                int x1 = static_cast<int>((v1.x + 1.) * w / 2.);
                int y1 = static_cast<int>((v1.y + 1.) * h / 2.);
                draw_line(x0, y0, x1, y1, Colors::White, *m_window);
            }
        }
    }

private:

    Window* m_window;
    Model m_model;
};

Logic::Logic(Window* window)
{
    m_impl = std::make_shared<Logic::Impl>(window);
}

void Logic::Update(int curr_time)
{
    m_impl->Update(curr_time);
}
