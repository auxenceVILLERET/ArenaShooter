#ifndef BULLETBAZOOKA_HPP_INCLUDED
#define BULLETBAZOOKA_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(BulletBazooka, Projectile, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

GameObject* m_target = nullptr;

void Start() override
{
	m_MaxDistance = 100.f;
	m_dmgBullet = 150.f;
	m_pOwner->SetActive(false);
}

void Update() override
{
	Projectile::Update();

	if (m_target == nullptr || m_target->IsDestroyed())
		return;
	Vector3f32 toTarget = m_target->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition();
	toTarget.SelfNormalize();

	m_Direction = toTarget;

}

void Init(Vector3f32 dir,Vector3f32 pos, float32 speed) override
{
	Projectile::Init(dir,pos,speed);
}

END_SCRIPT
#endif // !BULLETBAZOOKA_HPP_INCLUDED
