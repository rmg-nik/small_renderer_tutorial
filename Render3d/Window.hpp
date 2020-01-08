#pragma once

#include <memory>
#include <cstdint>

namespace render3d
{
    struct Color
    {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a = 255;
    };

    namespace Colors
    {
        extern const Color White;
        extern const Color Black;
        extern const Color Red;
        extern const Color Green;
        extern const Color Blue;
    }

    class Window
    {
    public:

        Window();

        int Init(int width, int height, bool flip_y);

        void Clear(const Color& color);

        void Flush();

        void DrawPixel(int x, int y, const Color& color);

        int GetWidth() const;

        int GetHeight() const;

    private:

        class Impl;
        std::shared_ptr<Impl> m_impl;
    };
}
