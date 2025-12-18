#ifndef ENERGYORB_HPP_INCLUDED
#define ENERGYORB_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Player.hpp"

using namespace gce;

DECLARE_SCRIPT(EnergyOrb, ScriptFlag::CollisionEnter)
	

void CollisionEnter(GameObject* other) override
{
	m_pOwner->SetName("Energy Orb");
	Console::Log(m_pOwner->GetName());
	if (other->GetName() == "Player")
	{
		other->GetScript<Player>()->m_energyOrbs += 1;
		m_pOwner->Destroy();
	}
	if (other->GetName() == "Boss bullet")
	{
		m_pOwner->Destroy();
	}
}


END_SCRIPT
#endif // !ENERGYORB_HPP_INCLUDED
