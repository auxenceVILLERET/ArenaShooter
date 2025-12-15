#ifndef CORE_STATE_MACHINE_H_INCLUDED
#define CORE_STATE_MACHINE_H_INCLUDED

#include <functional>

namespace gce
{

	class GameObject;

	using OnBegin = std::function<void()>;
	using OnUpdate = std::function<void()>;
	using OnEnd = std::function<void()>;

	struct StateMachine
	{
		StateMachine();

		struct Condition
		{
			std::function<bool()> condition;
		};

		struct Transition
		{
			Vector<Condition> conditions;
			String fromTarget;
			String toTarget;
		};

		struct Action
		{
			OnBegin onBegin;
			OnUpdate onUpdate;
			OnEnd onEnd;

			Vector<Transition> m_transitions;
		};

		void AddAction(String& name, Action action = {});
		void AddAction(String& name, OnBegin pOnBegin = nullptr, OnUpdate pOnUpdate = nullptr, OnEnd pOnEnd = nullptr);

		void SetOnBeginAction(String& name, OnBegin pOnBegin);
		void SetOnUpdateAction(String& name, OnUpdate pOnUpdate);
		void SetOnEndAction(String& name, OnEnd pOnEnd);

		void AddTransition(Transition transition);
		void AddTransition(Vector<Condition>& conditions, String& fromTarget, String& toTarget);

		void Update();

		void Transit(String target);
		
		String actualAction;

	private:

		String m_actualAction;
		UnorderedMap< String, Action> m_actions;

		friend class StatesSystem;
	};
}
#endif