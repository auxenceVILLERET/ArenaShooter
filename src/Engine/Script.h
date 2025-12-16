#ifndef ENGINE_COMPONENTS_SCRIPT_H
#define ENGINE_COMPONENTS_SCRIPT_H

#include <Containers/Vector.hpp>
#include <Containers/Queue.hpp>
#include "define.h"


enum class ScriptFlag : uint16
{
	Awake = 1 << 0,
	Start = 1 << 1,
	Update = 1 << 2,
	FixedUpdate = 1 << 3,
	Destroy = 1 << 4,

	CollisionEnter = 1 << 5,
	CollisionStay = 1 << 6,
	CollisionExit = 1 << 7,

	Collision2DEnter = 1 << 8,
	Collision2DStay  = 1 << 9,
	Collision2DExit  = 1 << 10,

	SetActive = 1 << 11,
	SetInnactive = 1 << 12,
};

inline uint16 operator|(ScriptFlag a, ScriptFlag b)
{
	return static_cast<uint16>(
		static_cast<uint16>(a) |
		static_cast<uint16>(b)
	);
}

inline uint16 operator&(ScriptFlag a, ScriptFlag b)
{
	return static_cast<uint16>(
		static_cast<uint16>(a) &
		static_cast<uint16>(b)
	);
}

inline uint16 operator|(uint16_t a, ScriptFlag b)
{
	return static_cast<uint16>(
		a |
		static_cast<uint16>(b)
	);
}

inline uint16 operator&(uint16_t a, ScriptFlag b)
{
	return static_cast<uint16>(
		a &
		static_cast<uint16>(b)
	);
}

inline uint16 operator|(ScriptFlag a, uint16_t b)
{
	return static_cast<uint16>(
		b |
		static_cast<uint16>(a)
	);
}

inline uint16 operator&(ScriptFlag a, uint16_t b)
{
	return static_cast<uint16>(
		b &
		static_cast<uint16>(a)
	);
}

#define DECLARE_SCRIPT( name, flags)                  \
    struct name: gce::Script                          \
    {												  \
		inline static const uint16 s_TYPE_ID = gce::Script::_StepId(); \
        name(uint16 f = 0): Script(f | flags){}

#define DECLARE_CHILD_SCRIPT(name, nameParent, flags)    \
	struct name : nameParent							\
	{													\
		inline static const uint16 s_TYPE_ID = gce::Script::_StepId(); \
		name(uint16 f = 0) : nameParent(f | flags) {}

#define END_SCRIPT };


namespace gce
{
	class GameObject;

	template <typename ...Args>
	struct Event;
	
	struct Script
	{
	public:
		Script() {};
		Script(uint16 flags) : m_flags(flags) {};
		~Script() = default;

	protected:
		GameObject* m_pOwner;
		uint16 m_id;
		inline static Vector<Script*> s_list;
		inline static Queue<uint16> s_idsList;
		inline static Queue<uint16> s_creationList;
		inline static Queue<uint16> s_deletionList;

		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void Destroy() {}

		virtual void CollisionEnter(GameObject* other) {}
		virtual void CollisionStay(GameObject* other) {}
		virtual void CollisionExit(GameObject* other) {}

		virtual void Collision2DEnter(GameObject* other) {}
		virtual void Collision2DStay(GameObject* other) {}
		virtual void Collision2DExit(GameObject* other) {}

		virtual void SetActiveEvent() {}
		virtual void SetInnactiveEvent() {}

		bool IsActive() const;
		void SetActive(bool active);
		static uint16 _StepId() { return s_nextId++; } // SHOULD NOT BE USED

	private:
		bool m_hadCollider = false;
		bool m_activated = true;
		uint16 m_flags = 0;
		inline static uint16 s_nextId = 0;
		void* m_pListeners[10];

		void Init();
		void UnInit();

		void OnAwake() { if (m_flags & ScriptFlag::Awake) Awake(); }
		void OnStart() { Start(); }
		void OnUpdate() { if ( IsActive() ) Update(); }
		void OnFixedUpdate() { if ( IsActive() ) FixedUpdate(); }
		void OnDestroy() { if ( IsActive() ) Destroy(); }
		void OnSetActive(bool active)
		{
			if ( IsActive() ) SetActiveEvent(); else SetInnactiveEvent();
		}

		void OnCollisionEnter(GameObject* const pOther) { if (IsActive()) CollisionEnter(pOther); }
		void OnCollisionStay(GameObject* const pOther) { if (IsActive()) CollisionStay(pOther); }
		void OnCollisionExit(GameObject* const pOther) { if (IsActive()) CollisionExit(pOther); }

		void OnCollision2DEnter(GameObject* const pOther) { if (IsActive()) Collision2DEnter(pOther); }
		void OnCollision2DStay(GameObject* const pOther) { if (IsActive()) Collision2DStay(pOther); }
		void OnCollision2DExit(GameObject* const pOther) { if (IsActive()) Collision2DExit(pOther); }

		friend class GameObject;
		friend class LifespanSystem;
	};
}

#endif
