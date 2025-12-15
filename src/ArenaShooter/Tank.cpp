#include "pch.h"
#include "Tank.h"
#if __has_include("Tank.g.cpp")
#include "Tank.g.cpp"
#endif

namespace winrt::MonProjet1::implementation
{
    int32_t Tank::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Tank::MyProperty(int32_t /*value*/)
    {
        throw hresult_not_implemented();
    }
}
