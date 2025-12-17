#include "pch.h"
#include "SliderComponent.h"

void gce::SliderComponent::SetPosition(Vector2f32 const& pos)
{
    if (useBarImage && barImageBrush != nullptr)
    {
        Vector2f32 scale = size / barImageBrush->GetSize();
        Vector2f32 tempPos = pos - (barImageBrush->GetSize() * scale / 2.f);
        barImageBrush->SetTransformMatrix({ tempPos.x, tempPos.y, 0.f }, { scale.x, scale.y, 1.f }, 0.f);
    }
    if (useFillImage && fillImageBrush != nullptr)
    {
        Vector2f32 scale = size / fillImageBrush->GetSize();
        Vector2f32 tempPos = pos - (fillImageBrush->GetSize() * scale / 2.f);
        fillImageBrush->SetTransformMatrix({ tempPos.x, tempPos.y, 0.f }, { scale.x, scale.y, 1.f }, 0.f);
    }
    position = pos;
}

void gce::SliderComponent::SetBaseColor(Color const& color)
{
    normalColor = color;
    highlightedColor = Color(Clamp(color.r - 10, 0, 255), Clamp(color.g - 10, 0, 255), Clamp(color.b - 10, 0, 255), 255);
    pressedColor = Color(Clamp(color.r - 100, 0, 255), Clamp(color.g - 100, 0, 255), Clamp(color.b - 100, 0, 255), 255);
    selectedColor = Color(Clamp(color.r - 10, 0, 255), Clamp(color.g - 10, 0, 255), Clamp(color.b - 10, 0, 255), 255);
    disabledColor = Color(Clamp(color.r - 55, 0, 255), Clamp(color.g - 55, 0, 255), Clamp(color.b - 55, 0, 255), 255);
    fillAreaColor = Color(Clamp(color.r - 55, 0, 255), Clamp(color.g - 55, 0, 255), Clamp(color.b - 55, 0, 255), 255);
}