#ifndef ENGINE_COMPONENT_UI_BASE_H_INCLUDED
#define ENGINE_COMPONENT_UI_BASE_H_INCLUDED

#include "define.h"
#include "Base.h"
#include "Maths/Vector2.hpp"
#include "Systems/UiSystem.h"

namespace gce 
{


DECLARE_COMPONENT_BASE(UIBase, ComponentBase)
{
    public:
    Vector2f32 position;
    Vector2f32 size;
    Vector2f32 winRatio;

protected:
};
};


#endif