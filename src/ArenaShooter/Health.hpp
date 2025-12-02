#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "define.h"
#include "Script.h"

DECLARE_SCRIPT(Health, ScriptFlag::Start)

float32 maxHealth = 10.0f;
float32 health = 10.0f;

void TakeDamage(float32 damage)
{
	health -= damage;
	if (health <= 0.0f)
		health = 0.0f;
}

END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
