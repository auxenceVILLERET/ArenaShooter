#ifndef CORE_STATE_MACHINE_H_INCLUDED
#define CORE_STATE_MACHINE_H_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"

using OnBegin = void(*)();
using OnUpdate = void(*)();
using OnEnd = void(*)();

struct StateMachine
{
	struct Condition
	{
		bool(*condition)();
	};

	struct Transition
	{
		gce::Vector<Condition> conditions;
		String target;
	};

	struct Action
	{
		OnBegin onBegin;
		OnUpdate onUpdate;
		OnEnd onEnd;
	};

	void AddAction(String& name, Action action = {});
	void AddAction(String& name, OnBegin pOnBegin = nullptr, OnUpdate pOnUpdate = nullptr, OnEnd pOnEnd = nullptr);

	void SetOnBeginAction(String& name, OnBegin pOnBegin);
	void SetOnUpdateAction(String& name, OnUpdate pOnUpdate);
	void SetOnEndAction(String& name, OnEnd pOnEnd);

	void AddTransition(Transition transition);
	void AddTransition(gce::Vector<Condition>& conditions, String& target);

	void Update();

	void Transit(String& target);

	String actualAction;

private :

	String m_actualAction;

	UnorderedMap< String, Action> m_actions;
	gce::Vector<Transition> m_transitions;
};

#endif