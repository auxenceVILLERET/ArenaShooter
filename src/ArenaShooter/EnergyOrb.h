#ifndef ENERGYORB_HPP_INCLUDED
#define ENERGYORB_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"

struct Player; // forward declaration

using namespace gce;

DECLARE_SCRIPT(EnergyOrb, ScriptFlag::CollisionEnter | ScriptFlag::Update)

float32 scale = 1.f;

void Update() override;
void CollisionEnter(GameObject* other) override;

END_SCRIPT
#endif
