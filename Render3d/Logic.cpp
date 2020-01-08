#include <Render3d/Logic.hpp>
#include <Render3d/Window.hpp>
#include <Render3d/tinyrenderer/model.h>
#include <cmath>

using namespace render3d;

namespace
{
    std::uint8_t rand_uint8()
    {
        return static_cast<std::uint8_t>(rand() % 255);
    }

    double DEG_TO_RAD(double deg)
    {
        return atan(1.0) * 4.0 * deg / 180.0;
    }

    void draw_line(int x1, int y1, int x2, int y2, const Color& color, Window* window)
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
            transposed ? window->DrawPixel(y, x, color) : window->DrawPixel(x, y, color);
            error2 += derror2;
            if (error2 > dx)
            {
                error2 -= 2 * dx;
                y += (y2 > y1) ? 1 : -1;
            }
        }
    }

    void draw_line(const Vec2i& p1, const Vec2i& p2, const Color& color, Window* window)
    {
        draw_line(p1.x, p1.y, p2.x, p2.y, color, window);
    }

    void draw_triangle(const Vec2i& p1, const Vec2i& p2, const Vec2i& p3, const Color& color, Window* window)
    {
        draw_line(p1.x, p1.y, p2.x, p2.y, color, window);
        draw_line(p2.x, p2.y, p3.x, p3.y, color, window);
        draw_line(p3.x, p3.y, p1.x, p1.y, color, window);
    }

    void draw_triangle_filled(Vec2i p1, Vec2i p2, Vec2i p3, const Color& color, Window* window)
    {
        if (p1.y == p2.y && p1.y == p3.y)
            return;

        if (p1.y > p2.y)
            std::swap(p1, p2);
        if (p1.y > p3.y)
            std::swap(p1, p3);
        if (p2.y > p3.y)
            std::swap(p2, p3);

        for (int y = p1.y; y <= p3.y; ++y)
        {
            const bool first_half = y < p2.y;
            const int x_start = (p3.x - p1.x) * (y - p1.y) / (p3.y - p1.y) + p1.x;
            const int x_end = first_half
                ? (p1.y == p2.y ? p1.x : ((p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y) + p1.x))
                : (p2.y == p3.y ? p3.x : ((p2.x - p3.x) * (y - p3.y) / (p2.y - p3.y) + p3.x));
            draw_line(x_start, y, x_end, y, color, window);
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

    void DrawSinusoid(int curr_time)
    {
        int w = m_window->GetWidth();
        int h = m_window->GetHeight();
        
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
            draw_line(x, y, prev_x, prev_y, Colors::Green, m_window);
            //m_window->DrawPixel(x, y, Colors::Blue);
            prev_x = x;
            prev_y = y;
        }
    }

    void DrawFace()
    {
        const int w = m_window->GetWidth();
        const int h = m_window->GetHeight();
        const Vec3f light_dir(0, 0, -1);

        for (int i = 0; i < m_model.nfaces(); i++)
        {
            std::vector<int> face = m_model.face(i);
            Vec2i screen_coords[3];
            Vec3f world_coords[3];
            for (int j = 0; j < 3; j++) {
                Vec3f v = m_model.vert(face[j]);
                screen_coords[j] = Vec2i((v.x + 1.) * w / 2., (v.y + 1.) * h / 2.);
                world_coords[j] = v;
            }
            Vec3f normal = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
            normal.normalize();
            float intensity = normal * light_dir;
            if (intensity > 0) {
                draw_triangle_filled(screen_coords[0], screen_coords[1], screen_coords[2],
                    {
                    static_cast<std::uint8_t>(intensity * 255),
                    static_cast<std::uint8_t>(intensity * 255),
                    static_cast<std::uint8_t>(intensity * 255)
                    }
                , m_window);
            }
        }
    }

    void DrawTriangles()
    {
        Vec2i t0[3] = { Vec2i(10, 70) * 4,   Vec2i(50, 160) * 4,  Vec2i(70, 80) * 4 };
        Vec2i t1[3] = { Vec2i(180, 50) * 4,  Vec2i(150, 1) * 4,   Vec2i(70, 180) * 4 };
        Vec2i t2[3] = { Vec2i(180, 150) * 4, Vec2i(120, 160) * 4, Vec2i(130, 180) * 4 };


        draw_triangle_filled(t0[0], t0[1], t0[2], Colors::Red, m_window);
        draw_triangle_filled(t1[0], t1[1], t1[2], Colors::White, m_window);
        draw_triangle_filled(t2[0], t2[1], t2[2], Colors::Green, m_window);
    }

    void Update(int curr_time)
    {
        DrawFace();
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
