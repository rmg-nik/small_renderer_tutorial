#pragma once

#include <memory>

namespace render3d
{
    class Window;
    class Logic
    {
    public:

        Logic(Window* window);

        void Update(int current_time);

    private:

        class Impl;
        std::shared_ptr<Impl> m_impl;
    };
}
