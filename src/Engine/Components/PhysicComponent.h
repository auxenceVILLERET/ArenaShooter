#ifndef ENGINE_PHYSIC_COMPONENT_H_INCLUDED
#define ENGINE_PHYSIC_COMPONENT_H_INCLUDED

#include "define.h"
#include "Base.h"
#include "Maths/MathsFunctions.hpp"
#include "Maths/Physics.h"

namespace gce {

    DECLARE_COMPONENT(PhysicComponent, ComponentBase)
    {
    public:
        PhysicComponent();
        PhysicComponent(uint16 maxNbForces);
        ~PhysicComponent() = default;

        void InitializeLists();

        // ... (Vos méthodes AddForce existantes inchangées) ...
        void AddConstantForce(Force & newForce);
        void AddForce(Force & newForce);
        void AddForceWithPosition(Force & newForce);
        void AddConstantForceWithPosition(Force & newForce);
        void AddTorque(Vector3f32 & torque);

        // === NOUVELLES MÉTHODES ===
        void SetGravityScale(float32 newGravityScale);
        void SetAirDragCoefficient(float32 newDragCoefficient);
        void SetStaticDragCoefficient(float32 newDragCoefficient);
        void SetDynamicDragCoefficient(float32 newDragCoefficient);
        void SetMass(float32 newMass);
        void SetBounciness(float32 newBounciness);
        void SetVelocity(Vector3f32 newVelocity);
        void SetRelativeCenterOfMass(Vector3f32 newCenterOfMass);

        // Gestion Trigger / Kinematic
        void SetIsTrigger(bool isTrigger);
        void SetIsKinematic(bool isKinematic);

        [[nodiscard]] float32 GetGravityScale() const;
        [[nodiscard]] Vector3f32 GetVelocity() const;
        [[nodiscard]] Vector3f32 GetAngularVelocity() const;

        // Accesseurs
        [[nodiscard]] bool IsTrigger() const;
        [[nodiscard]] bool IsKinematic() const;

    protected:
        Vector2<Vector3f32> UpdatePhysic(Vector3f32 & previousPosition, Vector3f32 & previousRotation, float32 deltaTime);

        //=== Settable members ===
        uint16 m_maxNbForces = 10;

        float32 m_gravityScale = 1.0f;
        float32 m_airDragCoefficient = 0.2f;
        float32 m_staticDragCoefficient = 5.f;
        float32 m_dynamicDragCoefficient = 0.2f;
        float32 m_angularFrictionCoeff = 0.5f;

        float32 m_bounciness = 0.7f;
        float32 m_mass = 20.f;

        // === NOUVEAUX BOOLÉENS ===
        bool m_isTrigger = false;    // Si true : collision détectée mais pas de réponse physique
        bool m_isKinematic = false;  // Si true : l'objet ne bouge pas via la physique (Newton ignoré)

        Vector3f32 m_centerOfMass{ 0,0,0 };
        Vector3f32 m_velocity{ 0,0,0 };
        Vector3f32 m_angularVelocity{ 0,0,0 };

        float32 m_momentOfInertia = 2.f;

        //=== Other members ===
        Vector3f32 m_staticForce{ 0,0,0 };

        Vector<Force> m_constForceList;
        Vector<Force> m_forceList;
        Vector<Force> m_appliedTorqueForceList;
        Vector<Vector3f32> m_torqueList;

        friend class PhysicSystem;
    };

}
#include "PhysicComponent.inl"
#endif