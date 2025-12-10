#ifndef ENGINE_COLLIDER_BASE_3D_H_INCLUDED
#define ENGINE_COLLIDER_BASE_3D_H_INCLUDED

#include "define.h"
#include "Base.h"
#include "Maths/Vector3.h"
#include "Maths/Physics.h"
#include <Event.hpp>
#include "Raycast.h"

namespace gce {

	
class GameObject;

enum CollisionType
{
	SphereSphere,BoxBox,SphereBox,
	CircleCircle, CircleSquare, SquareSquare
};
struct CollisionPair
{
	CollisionType type;
	Component* A;
	Component* B;
	bool physCompFlag;
	CollideResult result;
};

struct CollisionPair2D
{
	CollisionType type;
	Component* A;
	Component* B;
	bool physCompFlag;
	CollideResult2D result;
};


DECLARE_COMPONENT_BASE( ColliderBase3D, ComponentBase )
{
	virtual bool RaycastCollider(Ray const& ray, RaycastHit & hitInfo, float32 maxDistance) = 0;
	Event<GameObject*> collisionEnterEvent;
	Event<GameObject*> collisionStayEvent;
	Event<GameObject*> collisionExitEvent;

    Vector3f32 m_localOffSet{ 0,0,0 };
    bool isTrigger = false;

    Vector<uint32> m_wasCollidingID; //TODO change to macro


    friend class PhysicSystem;

protected:
	virtual void UpdateColliderComponent( uint8 dirty ) = 0;
};


}

#endif