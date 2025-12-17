#ifndef ENGINE_SLIDER_H_INCLUDED
#define ENGINE_SLIDER_H_INCLUDED

#include "define.h"
#include "Base.h"
#include <Core/Maths/Vector2.hpp>
#include <Core/Maths/Color.h>
#include <Render.h>

#include "GameObject.h"
#include "UiBase.h"

namespace gce

{
    DECLARE_COMPONENT(SliderComponent, UIBase)
    {
        enum SLIDER_ORIENTATION : uint8
        {
            LEFT_TO_RIGHT = 0b0001,
            RIGHT_TO_LEFT = 0b0010,
            UP_TO_DOWN = 0b0100,
            DOWN_TO_UP = 0b1000,

            HORIZONTAL = LEFT_TO_RIGHT | RIGHT_TO_LEFT,
            VERTICAL = UP_TO_DOWN | DOWN_TO_UP
        };

        enum SLIDER_SHAPE : uint8
        {
            ROUND = 0b001,
            ROUNDED_RECTANGLE = 0b010,
            RECTANGLE = 0b100

        };

        int anchor = 1;

        float32 max = 1.f;
        float32 min = 0.f;

        Vector2f32 offset = { 0.f, 0.f };
        Vector2f32 handleSize = { 0.f, 0.f };
        Vector2f32 handlePosition = { 0.f, 0.f };

        float32* p_targetValue = nullptr;
        float32 value = 0.f;
        float32 interval = 0.1f;

        bool isHandleMoved = true;
        bool isHandleHovered = false;
        bool isBarHovered = false;
        bool showHandle = true;
        bool isDraggingHandle = false;

        bool useHandleImage = false;
        bool useFillImage = false;
        bool useBarImage = false;
        BitMapBrush* handleImageBrush = nullptr;
        BitMapBrush* fillImageBrush = nullptr;
        BitMapBrush* barImageBrush = nullptr;

        SLIDER_ORIENTATION orientation = LEFT_TO_RIGHT;

        //Shapes
        SLIDER_SHAPE sliderShape = ROUND;

        //Colors
        Color borderColor = Color(0, 0, 0, 0);

        //Handle
        Color normalColor = Color::White;
        Color highlightedColor = Color(245, 245, 245, 255);
        Color pressedColor = Color(155, 155, 155, 255);
        Color selectedColor = Color(245, 245, 245, 255);
        Color disabledColor = Color(200, 200, 200, 255);

        //Backround
        Color backroundColor = Color(245, 245, 245, 0);

        //Fill Area
        Color fillAreaColor = Color(200, 200, 200, 0);

        void SetPosition(Vector2f32 const& pos);
        void SetBaseColor(Color const& color);

    private:

        friend class UiSystem;
    };
}

#endif