#include "pch.h"
#include "PhysicComponent.h"
#include "SphereCollider.h"
#include "CircleCollider2D.h"
#include "BoxCollider.h"
#include "BoxCollider2D.h"
#include "GameObject.h"
#include "Core/Maths/Physics.h"

namespace gce {

    PhysicComponent::PhysicComponent()
    {
        InitializeLists();
    }

    PhysicComponent::PhysicComponent(uint16 maxNbForces)
    {
        m_maxNbForces = maxNbForces;
        InitializeLists();
    }

    void PhysicComponent::InitializeLists()
    {
        m_constForceList.Reserve(m_maxNbForces);
        m_forceList.Reserve(m_maxNbForces);
        m_appliedTorqueForceList.Reserve(m_maxNbForces);
        m_torqueList.Reserve(m_maxNbForces);
    }

    void PhysicComponent::AddConstantForce(Force& newForce)
    {
        if (m_constForceList.Size() < m_constForceList.Capacity())
            m_constForceList.PushBack(newForce);
    }

    void PhysicComponent::AddForce(Force& newForce)
    {
        if (m_forceList.Size() < m_forceList.Capacity())
            m_forceList.PushBack(newForce);
    }

    void PhysicComponent::AddForceWithPosition(Force& newForce)
    {
        newForce.useApplicationPoint = true;
        AddForce(newForce);
    }

    void PhysicComponent::AddConstantForceWithPosition(Force& newForce)
    {
        newForce.useApplicationPoint = true;
        AddConstantForce(newForce);
    }

    void PhysicComponent::AddTorque(Vector3f32& torque)
    {
        if (m_torqueList.Size() < m_torqueList.Capacity())
            m_torqueList.PushBack(torque);
    }

    // === IMPL�MENTATION DES NOUVEAUX SETTERS/GETTERS ===

    void PhysicComponent::SetIsTrigger(bool isTrigger)
    {
        m_isTrigger = isTrigger;
    }

    void PhysicComponent::SetIsKinematic(bool isKinematic)
    {
        m_isKinematic = isKinematic;
        if (m_isKinematic)
        {
            // Optionnel : Si on devient Kinematic, on annule souvent la v�locit� courante
            m_velocity = { 0,0,0 };
            m_angularVelocity = { 0,0,0 };
        }
    }

    bool PhysicComponent::IsTrigger() const
    {
        return m_isTrigger;
    }

    bool PhysicComponent::IsKinematic() const
    {
        return m_isKinematic;
    }


    // === MISE � JOUR DE UpdatePhysic ===

    Vector2<Vector3f32> PhysicComponent::UpdatePhysic(Vector3f32& previousPosition, Vector3f32& previousRotation, float32 deltaTime)
    {
        // LOGIC KINETIC (KINEMATIC)
        // Si l'objet est Kinematic, les forces ne l'affectent pas. 
        // On nettoie les listes et on retourne la position inchang�e.
        if (m_isKinematic)
        {
            m_forceList.Clear();
            m_torqueList.Clear();
            m_staticForce = { 0.f,0.f,0.f };
            m_velocity = { 0.f, 0.f, 0.f }; // S�curit�
            return { previousPosition, previousRotation };
        }

        // --- Calcul standard pour objets dynamiques ---

        Vector3f32 totalForce(0.f, 0.f, 0.f);
        Vector3f32 totalTorque(0.f, 0.f, 0.f);

        // Correction du bug d'indexation ici : on utilise 'f' pour le CalculateTorque
        for (uint16 i = 0; i < m_constForceList.Size(); i++)
        {
            Force const& f = m_constForceList[i];
            Vector3f32 forceVec = f.direction * f.norm;

            if (f.useApplicationPoint)
            {
                // CORRECTION: Utiliser 'f' et non m_forceList[i]
                Vector3f32 torque = Physics::CalculateTorque(f, m_centerOfMass);
                totalTorque += torque;
            }
            totalForce += forceVec;
        }

        for (uint16 i = 0; i < m_forceList.Size(); i++)
        {
            Force const& f = m_forceList[i];
            Vector3f32 forceVec = f.direction * f.norm;

            if (f.useApplicationPoint)
            {
                // CORRECTION: Utiliser 'f' ici aussi
                Vector3f32 torque = Physics::CalculateTorque(f, m_centerOfMass);
                totalTorque += torque;
            }
            totalForce += forceVec;
        }

        for (uint16 i = 0; i < m_torqueList.Size(); i++)
        {
            totalTorque += m_torqueList[i];
        }

        // Translation
        Physics::NewtonForceLaw(previousPosition, m_velocity, m_angularVelocity, totalForce, m_mass, m_gravityScale, m_airDragCoefficient, m_staticForce / m_mass, deltaTime);

        // Rotation
        Physics::NewtonTorqueLaw(previousRotation, m_angularVelocity, m_angularFrictionCoeff, totalTorque, m_momentOfInertia, deltaTime);

        // Nettoyage
        m_forceList.Clear();
        m_torqueList.Clear();
        m_staticForce = { 0.f,0.f,0.f };

        return { previousPosition, previousRotation };
    }

}