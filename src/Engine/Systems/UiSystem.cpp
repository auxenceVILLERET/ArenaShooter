#include "pch.h"
#include "UiSystem.h"

#include "InputsMethods.h"

#include "Components/TextRenderer.h"
#include "Components/ImageUI.h"
#include "Components/UIButton.h"
#include "GameObject.h"
#include "GameTransform.h"
#include "Components/SliderComponent.h"
#include "Components/UiBase.h"
#include "Maths/Quaternion.h"

#include "GameManager.h"

namespace gce {


void UiSystem::HandleUiUpdate()
{

	// Button
	for ( UIButton* const pButton : UIButton::s_list )
	{
		if ( pButton->IsActive() == false || pButton->m_created == false || pButton->disabled ) continue;

		Vector2i32 const& mousePosition = GetMousePosition();

		if ( mousePosition.x >= pButton->m_rect.left && mousePosition.x <= pButton->m_rect.right &&
				mousePosition.y >= pButton->m_rect.top && mousePosition.y <= pButton->m_rect.bottom )
		{
			if ( pButton->m_hover == false )
			{
				pButton->m_hover = true;
				pButton->m_ppActiveBrush = &pButton->pHoverBitMapBrush;
			}

			if ( pButton->m_pressed == false )
			{
				if ( GetButtonDown( Mouse::LEFT ) ) pButton->m_pressed = true;
				continue;
			}

			if ( GetButtonUp( Mouse::LEFT ) )
			{
				pButton->m_onClickEvent.Invoke();
				pButton->m_pressed = false;
			}

			continue;
		}

		if ( pButton->m_hover )
		{
			pButton->m_hover = false;
			pButton->m_ppActiveBrush = &pButton->pBitMapBrush;
		}

		if ( pButton->m_pressed && GetButtonUp( Mouse::LEFT ) )
			pButton->m_pressed = false;
	}

	// SLIDER
	{
		for (SliderComponent* const pSlider : SliderComponent::s_list)
		{
			if (!pSlider->showHandle && pSlider->handleSize != Vector2f32{ 0 , 0 }) pSlider->handleSize = { 0, 0 };
			if (pSlider->showHandle)
			{
				if (pSlider->orientation & SliderComponent::HORIZONTAL && pSlider->size.y > pSlider->handleSize.y)
					pSlider->handleSize.y = pSlider->size.y;
				if (pSlider->orientation & SliderComponent::VERTICAL && pSlider->size.x > pSlider->handleSize.x)
					pSlider->handleSize.x = pSlider->size.x;
			}

			Vector2i32 mousePos = GetMousePosition();

			bool isXLinedUp = (mousePos.x >= pSlider->position.x - pSlider->size.x / 2.f)
				&& (mousePos.x <= pSlider->position.x + pSlider->size.x / 2.f);
			bool isYLinedUp = (mousePos.y >= pSlider->position.y - pSlider->size.y / 2.f)
				&& (mousePos.y <= pSlider->position.y + pSlider->size.y / 2.f);


			float32 start = pSlider->position.x - pSlider->size.x / 2.f + pSlider->offset.x + pSlider->handleSize.x / 2.f;
			switch (pSlider->orientation)
			{
			case SliderComponent::RIGHT_TO_LEFT: start = pSlider->position.x + pSlider->size.x / 2.f - pSlider->offset.x - pSlider->handleSize.x / 2.f; break;
			case SliderComponent::DOWN_TO_UP:    start = pSlider->position.y + pSlider->size.y / 2.f - pSlider->offset.x - pSlider->handleSize.y / 2.f; break;
			case SliderComponent::UP_TO_DOWN:    start = pSlider->position.y - pSlider->size.y / 2.f + pSlider->offset.x + pSlider->handleSize.y / 2.f; break;
			}

			float32 size = pSlider->size.x - pSlider->offset.x - pSlider->offset.y - pSlider->handleSize.x;
			if (pSlider->orientation & SliderComponent::VERTICAL) size = pSlider->size.y - pSlider->offset.x - pSlider->offset.y - pSlider->handleSize.y;
			float32 range = pSlider->max - pSlider->min;

			bool isXLinedUpHandle = (mousePos.x >= pSlider->handlePosition.x - pSlider->handleSize.x / 2.f)
				&& (mousePos.x <= pSlider->handlePosition.x + pSlider->handleSize.x / 2.f);
			bool isYLinedUpHandle = (mousePos.y >= pSlider->handlePosition.y - pSlider->handleSize.y / 2.f)
				&& (mousePos.y <= pSlider->handlePosition.y + pSlider->handleSize.y / 2.f);

			bool isXLinedUpOffset = (mousePos.x >= pSlider->position.x - pSlider->size.x / 2.f + pSlider->offset.x + pSlider->handleSize.x / 2)
				&& (mousePos.x <= pSlider->position.x + pSlider->size.x / 2.f - pSlider->offset.y - pSlider->handleSize.x / 2);
			bool isYLinedUpOffset = (mousePos.y >= pSlider->position.y - pSlider->size.y / 2.f + pSlider->offset.x + pSlider->handleSize.y / 2)
				&& (mousePos.y <= pSlider->position.y + pSlider->size.y / 2.f - pSlider->offset.y - pSlider->handleSize.y / 2);

			pSlider->isBarHovered = isXLinedUp && isYLinedUp;
			pSlider->isHandleHovered = isXLinedUpHandle && isYLinedUpHandle;
			bool isClickableHovered = (pSlider->orientation & SliderComponent::HORIZONTAL ? isXLinedUpOffset && isYLinedUp : isXLinedUp && isYLinedUpOffset);


			if (GetButtonDown(Mouse::LEFT))
			{
				if (pSlider->isHandleHovered || isClickableHovered || pSlider->isBarHovered)
				{
					pSlider->isDraggingHandle = true;
					if (isClickableHovered)		  pSlider->isHandleMoved = true;
				}
			}
			else if (GetButton(Mouse::LEFT))
			{
				if ((isClickableHovered || pSlider->isDraggingHandle))
				{
					pSlider->isHandleMoved = true;
					if (pSlider->orientation & pSlider->HORIZONTAL) pSlider->handlePosition.x = mousePos.x;
					if (pSlider->orientation & pSlider->VERTICAL) pSlider->handlePosition.y = mousePos.y;
				}
			}
	

			if (pSlider->isHandleMoved)
			{
				switch (pSlider->orientation)
				{
				case SliderComponent::LEFT_TO_RIGHT:
					if (pSlider->handlePosition.x < start)			pSlider->handlePosition.x = start;
					if (pSlider->handlePosition.x > start + size)	pSlider->handlePosition.x = start + size;
					pSlider->value = (pSlider->handlePosition.x - start) / size * range + pSlider->min;
					break;
				case SliderComponent::RIGHT_TO_LEFT:
					if (pSlider->handlePosition.x > start)			pSlider->handlePosition.x = start;
					if (pSlider->handlePosition.x < start - size)	pSlider->handlePosition.x = start - size;
					pSlider->value = (start - pSlider->handlePosition.x) / size * range + pSlider->min;
					break;
				case SliderComponent::UP_TO_DOWN:
					if (pSlider->handlePosition.y < start)			pSlider->handlePosition.y = start;
					if (pSlider->handlePosition.y > start + size)	pSlider->handlePosition.y = start + size;
					pSlider->value = (pSlider->handlePosition.y - start) / size * range + pSlider->min;
					break;
				case SliderComponent::DOWN_TO_UP:
					if (pSlider->handlePosition.y > start)			pSlider->handlePosition.y = start;
					if (pSlider->handlePosition.y < start - size)	pSlider->handlePosition.y = start - size;
					pSlider->value = (start - pSlider->handlePosition.y) / size * range + pSlider->min;
					break;
				}
				if (pSlider->p_targetValue != nullptr) *pSlider->p_targetValue = pSlider->value;

				if (!GetButtonDown(Mouse::LEFT) && !GetButton(Mouse::LEFT))
				{
					// Snap value to correct interval
					if (pSlider->value > pSlider->max)	pSlider->value = pSlider->max;
					if (pSlider->value < pSlider->min)	pSlider->value = pSlider->min;

					float32 temp = pSlider->min;
					while (temp <= pSlider->value)
					{
						if (pSlider->value - temp < pSlider->interval / 2.f) break;
						temp += pSlider->interval;
					}
					pSlider->value = temp;
					pSlider->isDraggingHandle = false;
					pSlider->isHandleMoved = false;
				}

				pSlider->handlePosition = { start + (pSlider->value - pSlider->min) * (size / range),
				pSlider->position.y };
				switch (pSlider->orientation)
				{
				case SliderComponent::RIGHT_TO_LEFT:
					pSlider->handlePosition = { start - (pSlider->value - pSlider->min) * size / range, pSlider->position.y };
					break;
				case SliderComponent::UP_TO_DOWN:
					pSlider->handlePosition = { pSlider->position.x, start + (pSlider->value - pSlider->min) * size / range };
					break;
				case SliderComponent::DOWN_TO_UP:
					pSlider->handlePosition = { pSlider->position.x, start - (pSlider->value - pSlider->min) * size / range };
					break;
				}

				if (pSlider->useHandleImage && pSlider->handleImageBrush != nullptr)
				{
					Vector2f32 scale = pSlider->handleSize / pSlider->handleImageBrush->GetSize();
					Vector2f32 pos = pSlider->handlePosition - (pSlider->handleImageBrush->GetSize() * scale / 2.f);
					pSlider->handleImageBrush->SetTransformMatrix({ pos.x, pos.y, 0.f }, { scale.x, scale.y, 1.f }, 0.f);
				}
			}
		}
	}
}


void UiSystem::HandleUiRendering()
{
	Window* const pWindow = GameManager::GetWindow();
	pWindow->BeginUI();

	for (ImageUI* const pImage : ImageUI::s_list)
	{
		if (pImage->IsActive() == false || pImage->m_created == false)
			continue;

		pWindow->SetActiveBrush(*pImage->btmBrush);
		pWindow->DrawShapeUI(*pImage->rectImage, 1);
	}

	// SLIDER
	{
		for (SliderComponent* const pSlider : SliderComponent::s_list)
		{
			if (pSlider->IsActive() == false )
				continue;

			ColorBrush backroundColor(pSlider->backroundColor, 1);
			ColorBrush normalColor(pSlider->normalColor, 1);
			ColorBrush highlightedColor(pSlider->highlightedColor, 1);
			ColorBrush borderColor(pSlider->borderColor, 1);
			ColorBrush fillAreaColor(pSlider->fillAreaColor, 1);
			ColorBrush pressedColor(pSlider->pressedColor, 1);
			ColorBrush selectedColor(pSlider->selectedColor, 1);

			Vector2f32 barShapeStart = { pSlider->position.x - pSlider->size.x / 2.f, pSlider->position.y - pSlider->size.y / 2.f };
			Vector2f32 barShapeEnd = { pSlider->position.x + pSlider->size.x / 2.f, pSlider->position.y + pSlider->size.y / 2.f };
			Vector2f32 fillShapeStart = { pSlider->position.x - pSlider->size.x / 2.f, pSlider->position.y - pSlider->size.y / 2.f };
			Vector2f32 fillShapeEnd = { pSlider->handlePosition.x + pSlider->handleSize.x / 2.f, pSlider->position.y + pSlider->size.y / 2.f };
			switch (pSlider->orientation)
			{
			case SliderComponent::RIGHT_TO_LEFT:
				fillShapeStart = { pSlider->handlePosition.x - pSlider->handleSize.x / 2.f, pSlider->position.y - pSlider->size.y / 2.f };
				fillShapeEnd = { pSlider->position.x + pSlider->size.x / 2.f, pSlider->position.y + pSlider->size.y / 2.f };
				break;
			case SliderComponent::UP_TO_DOWN:
				fillShapeEnd = { pSlider->position.x + pSlider->size.x / 2.f, pSlider->handlePosition.y + pSlider->handleSize.y / 2.f };
				break;
			case SliderComponent::DOWN_TO_UP:
				fillShapeStart = { pSlider->position.x - pSlider->size.x / 2.f, pSlider->handlePosition.y - pSlider->handleSize.y / 2.f };
				fillShapeEnd = { pSlider->position.x + pSlider->size.x / 2.f, pSlider->position.y + pSlider->size.y / 2.f };
				break;
			}

			// Base Bar Shape Selection
			ShapeUi* baseBarShape = new RectanglePosF(barShapeStart.x, barShapeStart.y, barShapeEnd.x, barShapeEnd.y);
			if (pSlider->sliderShape & SliderComponent::ROUNDED_RECTANGLE)
				baseBarShape = new RoundRectanglePosF(barShapeStart.x, barShapeStart.y, barShapeEnd.x, barShapeEnd.y,
					15.f, 15.f);
			if (pSlider->sliderShape & SliderComponent::ROUND)
				baseBarShape = new RoundRectanglePosF(barShapeStart.x, barShapeStart.y, barShapeEnd.x, barShapeEnd.y,
					45.f, 45.f);

			// Handle Shape Selection
			ShapeUi* handleShape = new Ellipse(pSlider->handlePosition, pSlider->handleSize.x / 2.f, pSlider->handleSize.y / 2.f);
			if (pSlider->sliderShape & SliderComponent::ROUNDED_RECTANGLE)
				handleShape = new RoundRectanglePosF(pSlider->handlePosition.x - pSlider->handleSize.x / 2.f,
					pSlider->handlePosition.y - pSlider->handleSize.y / 2.f,
					pSlider->handlePosition.x + pSlider->handleSize.x / 2.f,
					pSlider->handlePosition.y + pSlider->handleSize.y / 2.f,
					15.f, 15.f);
			else if (pSlider->sliderShape & SliderComponent::RECTANGLE)
				handleShape = new RectanglePosF(pSlider->handlePosition.x - pSlider->handleSize.x / 2.f,
					pSlider->handlePosition.y - pSlider->handleSize.y / 2.f,
					pSlider->handlePosition.x + pSlider->handleSize.x / 2.f,
					pSlider->handlePosition.y + pSlider->handleSize.y / 2.f);
			else if (pSlider->handleSize.x != pSlider->handleSize.y)
				handleShape = new RoundRectanglePosF(pSlider->handlePosition.x - pSlider->handleSize.x / 2.f,
					pSlider->handlePosition.y - pSlider->handleSize.y / 2.f,
					pSlider->handlePosition.x + pSlider->handleSize.x / 2.f,
					pSlider->handlePosition.y + pSlider->handleSize.y / 2.f,
					45.f, 45.f);

			// Fill Bar Shape
			ShapeUi* fillRectShape = new RectanglePosF(fillShapeStart.x, fillShapeStart.y, fillShapeEnd.x, fillShapeEnd.y);
			if (pSlider->sliderShape & SliderComponent::ROUNDED_RECTANGLE)
				fillRectShape = new RoundRectanglePosF(fillShapeStart.x, fillShapeStart.y, fillShapeEnd.x, fillShapeEnd.y,
					15.f, 15.f);
			if (pSlider->sliderShape & SliderComponent::ROUND)
				fillRectShape = new RoundRectanglePosF(fillShapeStart.x, fillShapeStart.y, fillShapeEnd.x, fillShapeEnd.y,
					45.f, 45.f);

			if (pSlider->useBarImage && pSlider->barImageBrush != nullptr)
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(*pSlider->barImageBrush);
			else
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(backroundColor);
			GameManager::s_pInstance->m_pWindow->DrawShapeUI(*baseBarShape, true);

			if (pSlider->useFillImage && pSlider->fillImageBrush != nullptr)
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(*pSlider->fillImageBrush);
			else
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(fillAreaColor);
			GameManager::s_pInstance->m_pWindow->DrawShapeUI(*fillRectShape, true);

			GameManager::s_pInstance->m_pWindow->SetActiveBrush(borderColor);
			GameManager::s_pInstance->m_pWindow->DrawShapeUI(*baseBarShape, false);

			if (!pSlider->showHandle) continue;

			if (pSlider->isDraggingHandle)
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(pressedColor);
			else if (pSlider->isHandleHovered)
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(highlightedColor);
			else
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(normalColor);
			if (pSlider->useHandleImage && pSlider->handleImageBrush != nullptr)
				GameManager::s_pInstance->m_pWindow->SetActiveBrush(*pSlider->handleImageBrush);

			GameManager::s_pInstance->m_pWindow->DrawShapeUI(*handleShape, true);
			GameManager::s_pInstance->m_pWindow->SetActiveBrush(borderColor);
			GameManager::s_pInstance->m_pWindow->DrawShapeUI(*handleShape, false);
		}
	}

	// Button
	for ( UIButton* const pButton : UIButton::s_list )
	{
		if ( pButton->IsActive() == false || pButton->m_created == false ) continue;

		pWindow->SetActiveBrush( **pButton->m_ppActiveBrush );
		pWindow->DrawShapeUI( pButton->m_rect, true );
	}

	// TextRenderer
	for ( TextRenderer* const pTextRenderer : TextRenderer::s_list )
	{
		if ( pTextRenderer->IsActive() == false || pTextRenderer->m_created == false ) continue;

		pWindow->SetActiveBrush( *pTextRenderer->pBrush );
		pWindow->DrawTextUI( pTextRenderer->text, *pTextRenderer->pFont, *pTextRenderer->rectPosF );
	}

	pWindow->EndUI();
}


}
