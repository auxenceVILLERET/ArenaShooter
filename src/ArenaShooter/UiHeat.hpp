#ifndef UIHEAT_HPP_INCLUDED
#define UIHEAT_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"

DECLARE_SCRIPT(UiHeat, ScriptFlag::Update)

ImageUI* UiHeatH= nullptr;

void Update() override
{
	
}

void UiHeatBar(float32 m_heat, Vector3f32 pos)
{
	//UiHeatH->InitializeImage({ pos.x, pos.y }, { 118,(510 * m_heat / 100.f) }, 1.f);
	UiHeatH->btmBrush->SetTransformMatrix(pos, { 1.f , m_heat / 100.f, 1.f }, 180);

}


END_SCRIPT
#endif // !UIHEAT_HPP_INCLUDED
