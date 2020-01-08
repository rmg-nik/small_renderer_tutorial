#pragma once

#include <memory>

namespace render3d
{
    class Application
    {
    public:

        Application();

        int Run();

    private:

        class Impl;
        std::shared_ptr<Impl> m_impl;
    };
}
