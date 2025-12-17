#ifndef UIENERGYORB_HPP_INCLUDED
#define UIENERGYORB_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"

using namespace gce;

DECLARE_SCRIPT(UiEnergyOrb, ScriptFlag::Update)

ImageUI* UiEnergyH = nullptr;

void Update() override
{

}

void UiEnergyOrbBar(int8 energyOrbs, Vector3f32 pos)
{
	Vector3f32 offset = { 64.f,64.f,0.f };
	offset -= offset * energyOrbs / 8.f;
	UiEnergyH->InitializeImage({ pos.x + offset.x, pos.y + offset.y }, { 128,128 }, 1.f);
	UiEnergyH->btmBrush->SetTransformMatrix(pos + offset , {1.f * energyOrbs / 8.f , 1.f * energyOrbs / 8.f ,1.f * energyOrbs / 8.f}, 0);
}


END_SCRIPT
#endif // !UIHEAT_HPP_INCLUDED
