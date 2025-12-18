#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>
#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"
#include "Shapes.h"
#include "Rifle.hpp"
#include "Shotgun.hpp"
#include "Handgun.hpp"
#include "Bazooka.h"
#include "Health.h"
#include "WeaponController.hpp"
#include "BulletDrone.hpp"
#include "BulletTank.hpp"
#include "SceneManager.h"
#include "CustomScene.h"
#include "UiHp.hpp"
#include "UiHeat.hpp"
#include "UiEnergyOrb.hpp"

using namespace gce;

DECLARE_SCRIPT(Player, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionStay | ScriptFlag::CollisionEnter | ScriptFlag::CollisionExit | ScriptFlag::SetActive)

float32 m_speed = 8;
float32 m_jumpForce = 40000;
float32 m_boostForce = 500;
float32 m_airMovementForce = m_jumpForce / 15;
Vector3f32 m_currentOffset = { 0,0,0 };
GameObject* m_camObj = nullptr;
Camera* m_camera = nullptr;
Rifle* m_rifle = nullptr;
Shotgun* m_shotgun = nullptr;
Handgun* m_handgun = nullptr;
Bazooka* m_bazooka = nullptr;
UiHp* m_uiHp = nullptr;
UiHeat* m_uiHeat = nullptr;
UiEnergyOrb* m_uiEnergyOrb = nullptr;

WeaponController* m_weaponController = nullptr;

CustomScene* m_customScene = nullptr;

Health<int>* m_health = nullptr;
int8 m_energyOrbs = 0;
int8 m_maxEnergyOrbs = 8;

void Start() override
{
	m_health = new Health<int>(5);

	GameObject& cam = GameObject::Create(m_pOwner->GetScene());
	cam.SetParent(*m_pOwner);
	cam.transform.SetLocalPosition({ 0.f, 0.8f, 0.f });
	m_camera = cam.AddComponent<Camera>();
	m_camObj = &m_customScene->AddObject();
	m_camObj->SetParent(*m_pOwner);
	m_camObj->transform.SetLocalPosition({ 0.f, 0.8f, 0.f });
	m_camera = m_camObj->AddComponent<Camera>();
	m_camera->SetMainCamera();
	m_camera->SetType(PERSPECTIVE);
	m_camera->perspective.fov = XM_PIDIV4;
	m_camera->perspective.nearPlane = 0.001f;
	m_camera->perspective.farPlane = 500.0f;
	m_camera->perspective.aspectRatio = 600.0f / 400.0f;
	m_camera->perspective.up = { 0.0f, 1.0f, 0.0f };
}

void SetActiveEvent() override
{

	Geometry* pRifleGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/rifle.obj");
	Texture* albedoRifle = new Texture(RES_PATH"res/ArenaShooter/Obj/rifle_color.png");
	Texture* roughRifle = new Texture(RES_PATH"res/ArenaShooter/Obj/rifle_rough.png");
	Texture* metalRifle = new Texture(RES_PATH"res/ArenaShooter/Obj/rifle_metal.png");
	Texture* normRifle = new Texture(RES_PATH"res/ArenaShooter/Obj/rifle_norm.png");
	Texture* aoRifle = new Texture(RES_PATH"res/ArenaShooter/Obj/rifle_ao.png");

	Geometry* pShotgunGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/shotgun_lower.obj");
	Texture* albedoShotgun = new Texture(RES_PATH"res/ArenaShooter/Obj/shotgun_color.png");
	Texture* roughShotgun = new Texture(RES_PATH"res/ArenaShooter/Obj/shotgun_Roughness.png");
	Texture* normalShotgun = new Texture(RES_PATH"res/ArenaShooter/Obj/shotgun_Normal.png");

	Geometry* pHandgunGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/handgun_lower.obj");
	Texture* albedoHandgun = new Texture(RES_PATH"res/ArenaShooter/Obj/handun_c.png");
	Texture* roughHandgun = new Texture(RES_PATH"res/ArenaShooter/Obj/handgun_rough.png");
	Texture* ambiantHandgun = new Texture(RES_PATH"res/ArenaShooter/Obj/handhun_ao.png");

	if (m_health != nullptr)
		m_health->Heal(5);
	m_pOwner->transform.SetWorldRotation({ 0,0,0 });

	GameObject& weaponControllerObj = m_customScene->AddObject();
	m_weaponController = weaponControllerObj.AddScript<WeaponController>();
	weaponControllerObj.SetParent(*m_pOwner);

	GameObject& hpUi = m_customScene->AddObject();
	ImageUI& uiImage = *hpUi.AddComponent<ImageUI>();
	Vector2f32 center = { 1920 / 2 , 1080 / 2 };
	Vector2f32 size = { 1920, 1080 };
	Vector2f32 posUi = center - size * 0.5f;
	uiImage.InitializeImage(posUi, size, 1.f);
	m_uiHp = hpUi.AddScript<UiHp>();
	m_uiHp->m_pPlayer = m_health;
	m_uiHp->UiHpImage = &uiImage;
	uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/stade1.png");

	GameObject& heatUi = m_customScene->AddObject();
	ImageUI& uiHeatImage = *heatUi.AddComponent<ImageUI>();
	Vector2f32 size2 = { 35, 153 };
	uiHeatImage.InitializeImage({ 1100, 480 }, size2, 1.f);
	uiHeatImage.btmBrush =  new BitMapBrush("res/ArenaShooter/barre_de_surcharge.png");
	uiHeatImage.btmBrush->SetTransformMatrix({ 1100, 480, 0 }, { 0.3f , 0.3f , 0.3f }, 0);


	GameObject& rifle = m_customScene->AddObject();
	MeshRenderer& meshProjectileRifle = *rifle.AddComponent<MeshRenderer>();
	meshProjectileRifle.pGeometry = pRifleGeo;
	meshProjectileRifle.pMaterial->albedoTextureID = albedoRifle->GetTextureID();
	meshProjectileRifle.pMaterial->useTextureAlbedo = 1;
	meshProjectileRifle.pMaterial->roughnessTextureID = roughRifle->GetTextureID();
	meshProjectileRifle.pMaterial->useTextureRoughness = 1;
	meshProjectileRifle.pMaterial->metalnessTextureID = metalRifle->GetTextureID();
	meshProjectileRifle.pMaterial->useTextureMetalness = 1;
	meshProjectileRifle.pMaterial->normalTextureID = normRifle->GetTextureID();
	meshProjectileRifle.pMaterial->useTextureNormal = 1;
	meshProjectileRifle.pMaterial->ambientTextureID = aoRifle->GetTextureID();
	meshProjectileRifle.pMaterial->useTextureAmbient = 1;

	m_rifle = rifle.AddScript<Rifle>();
	rifle.SetParent(*m_camObj);
	rifle.transform.SetWorldScale({ 0.8f,0.8f,0.8f });
	rifle.transform.SetLocalPosition({ 0.3f,-0.2f,0.5f });
	rifle.SetActive(false);
	m_rifle->m_pCustomScene = m_customScene;

	m_weaponController->AddWeapon(m_rifle);

	GameObject& shotgun = m_customScene->AddObject();
	MeshRenderer& meshProjectileShotgun = *shotgun.AddComponent<MeshRenderer>();
	meshProjectileShotgun.pGeometry = pShotgunGeo;
	meshProjectileShotgun.pMaterial->albedoTextureID = albedoShotgun->GetTextureID();
	meshProjectileShotgun.pMaterial->useTextureAlbedo = 1;
	meshProjectileShotgun.pMaterial->roughnessTextureID = roughShotgun->GetTextureID();
	meshProjectileShotgun.pMaterial->useTextureRoughness = 1;
	meshProjectileShotgun.pMaterial->normalTextureID = normalShotgun->GetTextureID();
	meshProjectileShotgun.pMaterial->useTextureNormal = 1;
	m_shotgun = shotgun.AddScript<Shotgun>();
	shotgun.SetParent(*m_camObj);
	shotgun.transform.SetWorldScale({ 1.1f,1.1f,1.1f });
	shotgun.transform.SetLocalPosition({ 0.7f,-0.5f,1.5f });
	shotgun.transform.SetLocalRotation({ 0.f, 0.f, 0.f });
	shotgun.SetActive(false);
	m_shotgun->m_pCustomScene = m_customScene;

	m_weaponController->AddWeapon(m_shotgun);

	GameObject& handgun = m_customScene->AddObject();
	MeshRenderer& meshProjectileHandgun = *handgun.AddComponent<MeshRenderer>();
	meshProjectileHandgun.pGeometry = pHandgunGeo;
	meshProjectileHandgun.pMaterial->albedoTextureID = albedoHandgun->GetTextureID();
	meshProjectileHandgun.pMaterial->useTextureAlbedo = 1;
	meshProjectileHandgun.pMaterial->roughnessTextureID = roughHandgun->GetTextureID();
	meshProjectileHandgun.pMaterial->useTextureRoughness = 1;
	meshProjectileHandgun.pMaterial->ambientTextureID = ambiantHandgun->GetTextureID();
	meshProjectileHandgun.pMaterial->useTextureAmbient = 1;
	m_handgun = handgun.AddScript<Handgun>();
	handgun.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	handgun.SetParent(*m_camObj);
	handgun.transform.SetLocalPosition({ 0.5f,-0.2f,1.f });
	handgun.SetActive(false);
	m_handgun->m_pCustomScene = m_customScene;

	m_weaponController->AddWeapon(m_handgun);

	GameObject& bazooka = m_customScene->AddObject();
	MeshRenderer& meshProjectileBazooka = *bazooka.AddComponent<MeshRenderer>();
	meshProjectileBazooka.pGeometry = SHAPES.CAPSULE;
	m_bazooka = bazooka.AddScript<Bazooka>();
	bazooka.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	bazooka.SetParent(*m_camObj);
	bazooka.transform.SetLocalPosition({ 0.3f,-0.3f,1.f });
	bazooka.SetActive(false);
	m_bazooka->m_pCustomScene = m_customScene;
	m_weaponController->AddWeapon(m_bazooka, false);
	
	GameObject& energyUiEmpty = m_customScene->AddObject();
	ImageUI& uiEnergyEmpty = *energyUiEmpty.AddComponent<ImageUI>();
	uiEnergyEmpty.InitializeImage({ 90, 880 }, { 128,128 }, 1.f);
	uiEnergyEmpty.btmBrush = new BitMapBrush("res/ArenaShooter/contours_bouboules.png");
	uiEnergyEmpty.btmBrush->SetTransformMatrix({ 90, 880, 0 }, { 1.f , 1.0f , 1.0f }, 0);

	GameObject& energyUi = m_customScene->AddObject();
	ImageUI& uiEnergy = *energyUi.AddComponent<ImageUI>();
	m_uiEnergyOrb = energyUi.AddScript<UiEnergyOrb>();
	m_uiEnergyOrb->UiEnergyH = &uiEnergy;
	uiEnergy.InitializeImage({ 90, 880 }, { 128,128 }, 1.f);
	uiEnergy.btmBrush = new BitMapBrush("res/ArenaShooter/bouboule.png");
	uiEnergy.btmBrush->SetTransformMatrix({ 90, 880, 0 }, { 1.f , 1.0f , 1.0f }, 0);

	GameObject& heatUiBar = m_customScene->AddObject();
	ImageUI& uiHeatBarImage = *heatUiBar.AddComponent<ImageUI>();
	m_uiHeat = heatUiBar.AddScript<UiHeat>();
	m_uiHeat->UiHeatH = &uiHeatBarImage;
	uiHeatBarImage.InitializeImage({ 1100, 630 }, { 44,7 }, 1.f);
	uiHeatBarImage.btmBrush = new BitMapBrush("res/ArenaShooter/curseur_barre_de_surcharge.png");
	uiHeatBarImage.btmBrush->SetTransformMatrix({ 1100, 630, 0 }, { 0.6f , 0.6f , 0.6f }, 0);

}

void Test()
{
	GameObject& testObject = m_customScene->AddObject();
	testObject.AddComponent<MeshRenderer>()->pGeometry = SHAPES.CUBE;

	testObject.transform.SetWorldPosition(m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * 2.f);
	
}

void AddEnergyOrb()
{
	if (m_energyOrbs < m_maxEnergyOrbs)
		m_energyOrbs++;
}

void Update() override
{
	m_deltaTime = GameManager::DeltaTime();
	RaycastUpdate();

	if (IsEnergyFull() == true)
	{
		m_weaponController->UnlockWeapon(3);
		m_energyOrbs = m_maxEnergyOrbs;
	}

	m_uiHeat->UiHeatBar(m_weaponController->GetCurrentWeapon()->GetHeat(),{1100,630,0});
	m_uiEnergyOrb->UiEnergyOrbBar(m_energyOrbs, {90, 880 ,0});

	if (m_health->GetHealth() <= 0)
	{
		m_pOwner->SetActive(false);
		SceneManager::GetInstance()->GetCurrentScene()->Empty(2);
		SceneManager::GetInstance()->ChangeScene(GAMEOVER);
	}
}

bool IsEnergyFull()
{
	if (m_energyOrbs >= m_maxEnergyOrbs)
		return true;
	else
		return false;
}

bool IsRising()
{
	if (m_pOwner->GetComponent<PhysicComponent>()->GetVelocity().y <= 0)
		return false;
	else
		return true;
}

bool IsGrounded()
{
	if (m_isGrounded == true)
		return true;
	else if (m_isGrounded == false)
		return false;
}

void GettingPunched()
{
	m_isGettingPunched = true;
}

void Jump()
{
	if (m_isGrounded)
	{
		Force jumpForce;
		jumpForce.direction = { 0, 1, 0};
		jumpForce.norm = m_jumpForce;
		
		Vector3f32 jumpDirection;
		jumpDirection = { m_currentOffset.x, 0, m_currentOffset.z };
		jumpForce.direction += jumpDirection;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(jumpForce);
	}
}

void BoostUp()
{
	if (m_isGrounded == false)
	{
		Force boostForce;
		boostForce.direction = { 0, 1, 0 };
		boostForce.norm = m_boostForce;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(boostForce);
	}
}

void Move(Vector3f32 direction)
{
	m_currentOffset = (m_pOwner->transform.GetLocalForward().Normalize() * direction.z + m_pOwner->transform.GetLocalRight().Normalize() * direction.x);
	Vector3f32 offset = m_currentOffset * m_speed;

	if(m_isGrounded == true)
	{
		PhysicComponent& phys = *m_pOwner->GetComponent<PhysicComponent>();
		Vector3f32 vel = phys.GetVelocity();
		phys.SetVelocity({ offset.x, vel.y , offset.z });
	}
	else if (m_isGrounded == false && m_isGettingPunched == false)
	{
		Force airMovementForce;
		airMovementForce.direction = m_currentOffset;
		airMovementForce.norm = m_airMovementForce;

		PhysicComponent& phys = *m_pOwner->GetComponent<PhysicComponent>();
		Vector3f32 vel = phys.GetVelocity();

		if (m_currentOffset.x > 0 && vel.x > offset.x )
			airMovementForce.direction.x = 0;
		else if (m_currentOffset.x < 0 && vel.x < offset.x)
			airMovementForce.direction.x = 0;
		if (m_currentOffset.z > 0 && vel.z > offset.z)
			airMovementForce.direction.z = 0;
		else if (m_currentOffset.z < 0 && vel.z < offset.z)
			airMovementForce.direction.z = 0;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(airMovementForce);
	}
}

void Rotate(Vector3f32 rotation)
{
	Vector3f32 tilt = rotation * m_deltaTime;

	m_pOwner->transform.LocalRotate(tilt);
}

void Die()
{
	m_health->TakeDamage(5);
}

void CollisionStay(GameObject* other) override
{
	
}

void CollisionEnter(GameObject* other)
{
	/*Console::Log("Touch");
	Console::Log(other->GetName());

	m_isGrounded = true;*/

	if (other->GetScript<BulletDrone>())
	{
		TakeDamage(other->GetScript<BulletDrone>()->GetDmgBullet());
	}
	if (other->GetScript<BulletTank>())
	{
		TakeDamage(other->GetScript<BulletTank>()->GetDmgBullet());
	}
	
}

void CollisionExit(GameObject* other) override
{
	/*Console::Log("Untouch");
	
	m_isGrounded = false;*/
}

WeaponController* GetWeaponController()
{
	return m_weaponController;
}

void RaycastUpdate()
{
	Ray ray;
	ray.origin = m_pOwner->transform.GetWorldPosition();
	ray.direction = m_pOwner->transform.GetWorldUp();
	ray.direction.y *= -1.f;

	float32 maxDistance = 1.1f;
	RaycastHit hitInfo;
	float32 distance = maxDistance;
	Vector3f32 hitPoint = ray.origin + ray.direction * distance;
	if (PhysicSystem::IntersectRay(ray, hitInfo, maxDistance))
	{
		if (hitInfo.pGameObject && hitInfo.pGameObject->HasComponent<MeshRenderer>())
		{
			//hitInfo.pGameObject->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = laserTexture.GetTextureID();
		}

		distance = hitInfo.distance;
		hitPoint = hitInfo.point;


		m_isGrounded = true;
		m_isGettingPunched = false;
	}
	else
	{
		m_isGrounded = false;
	}
}

void TakeDamage(int dmg)
{
	m_health->TakeDamage(dmg);
	m_uiHp->UiImageHp();
}

private:
	float32 m_deltaTime;

	bool m_isGrounded;
	bool m_isGettingPunched = false;


END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
