#ifndef UIHEAT_HPP_INCLUDED
#define UIHEAT_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"

using namespace gce;

DECLARE_SCRIPT(UiHeat, ScriptFlag::Update)

ImageUI* UiHeatH = nullptr;

void Update() override
{
	
}

void UiHeatBar(float32 m_heat, Vector3f32 pos)
{
	float32 y = pos.y - m_heat / 100.f * 153.f;
	UiHeatH->InitializeImage({ pos.x, y}, {44,7}, 1.f);
	UiHeatH->btmBrush->SetTransformMatrix({pos.x,y , pos.z}, {0.6f , 0.6f , 0.6f}, 0);
}


END_SCRIPT
#endif // !UIHEAT_HPP_INCLUDED
