#pragma once

#include "Tank.g.h"

namespace winrt::MonProjet1::implementation
{
    struct Tank : TankT<Tank>
    {
        Tank() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::MonProjet1::factory_implementation
{
    struct Tank : TankT<Tank, implementation::Tank>
    {
    };
}
