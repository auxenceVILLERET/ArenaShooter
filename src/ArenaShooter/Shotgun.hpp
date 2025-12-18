#ifndef SHOTGUN_HPP_INCLUDED
#define SHOTGUN_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Weapon.hpp"
#include "BulletShotgun.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Shotgun, Weapon, ScriptFlag::Start | ScriptFlag::Update)

float32 m_spreadAngle = 3.f; // Angle de dispersion des projectiles (horizontal)
float32 m_verticalSpread = 3.f; // Angle de dispersion verticale (au-dessus / en-dessous)
int m_numPellets = 8;         // Nombre de projectiles tir�s

void Start() override
{
    Weapon::Start();
    m_shotCooldown = 1.0f;     
    m_heatPerShot = 20.f;
    Geometry* pGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/laser.obj");
    Texture* albedo = new Texture(RES_PATH"res/ArenaShooter/Obj/LaserTxtBlue.png");

    for (int i = 0; i < 50; i++)
    {
        GameObject& bullet = m_pCustomScene->AddObject();

        MeshRenderer& meshProjectile = *bullet.AddComponent<MeshRenderer>();
        meshProjectile.pGeometry = pGeo;
        meshProjectile.pMaterial->albedoTextureID = albedo->GetTextureID();
        meshProjectile.pMaterial->useTextureAlbedo = 1.f;
        bullet.transform.SetWorldPosition({0.0f, 0.0f, 0.0f});
        bullet.transform.SetWorldScale({ 1.8f, 1.8f, 1.8f });
        bullet.SetName("Shotgun bullet");

        bullet.AddComponent<SphereCollider>();
        bullet.AddComponent<PhysicComponent>()->SetGravityScale(0.0f);
        
        m_pProjectiles.PushBack(bullet.AddScript<BulletShotgun>());
    }
}

bool Shoot() override
{
    if (!Weapon::Shoot())
        return false;
    

    Vector3f32 forward = m_pOwner->transform.GetWorldForward();
    Vector3f32 right = m_pOwner->transform.GetWorldRight();
    Vector3f32 up = m_pOwner->transform.GetWorldUp();

    for (int i = 0; i < m_numPellets; ++i)
    {
        // Calcul d'un angle al�atoire horizontal et vertical
        float32 horizontalAngle = ((rand() % 2001) - 1000) / 1000.f * m_spreadAngle; 
        float32 verticalAngle = ((rand() % 2001) - 1000) / 1000.f * m_verticalSpread; 

        // Calcul de la direction de tir
        Vector3f32 spreadDirection = GetSpreadDirection(horizontalAngle, verticalAngle);

        Vector3f32 worldDirection = forward + right * spreadDirection.x + up * spreadDirection.y;

        worldDirection.SelfNormalize();

        Projectile* proj = GetFirstAvailableProjectile();
        BulletShotgun* bulletShotgun = dynamic_cast<BulletShotgun*>(proj);

        if (bulletShotgun)
            bulletShotgun->Init(worldDirection,m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * 0.6f, 40.f);
    }
}

Vector3f32 GetSpreadDirection(float32 horizontalAngle, float32 verticalAngle)
{
    float32 horizontalRad = horizontalAngle * (PI / 180.f);
    float32 verticalRad = verticalAngle * (PI / 180.f);
    
    float32 cosHorizontal = cos(horizontalRad);
    float32 sinHorizontal = sin(horizontalRad);
    
    float32 cosVertical = cos(verticalRad);
    float32 sinVertical = sin(verticalRad);
    
    Vector3f32 direction;
    direction.x = cosVertical * sinHorizontal;
    direction.y = sinVertical;
    direction.z = cosVertical * cosHorizontal;
    
    return direction.Normalize();
}

END_SCRIPT

#endif // !SHOTGUN_HPP_INCLUDED
