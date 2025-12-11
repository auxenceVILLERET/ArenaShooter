#include "pch.h"
#include "StateMachine.h"
#include "GameManager.h"

namespace gce
{
	StateMachine::StateMachine()
	{
	}

	///////////////////////////////////////////////////////////////////////////////
	// @brief Adds a given action to the state machine. (Single Action version)
	// @param name The name of the action. 
	// @param action The action to be added.
	///////////////////////////////////////////////////////////////////////////////
	void StateMachine::AddAction(String& name, Action action)
	{
		m_actions[name] = action;
		if (m_actions.size() == 1)
		{
			m_actualAction = name;
			actualAction = m_actualAction;
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// @brief Adds a given action to the state machine. (Begin + Update + End  version)
	// @param name The name of the action. 
	// @param pOnBegin Pointer to the function to be called when the action begins.
	// @param pOnUpdate Pointer to the function to be called when the action is updated.
	// @param pOnEnd Pointer to the function to be called when the action ends.
	////////////////////////////////////////////////////////////////////////////////////////
	void StateMachine::AddAction(String& name, OnBegin pOnBegin, OnUpdate pOnUpdate, OnEnd pOnEnd)
	{
		m_actions[name] = { pOnBegin,pOnUpdate,pOnEnd };
		if (m_actions.size() == 1)
		{
			m_actualAction = name;
			actualAction = m_actualAction;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////
	// @brief Sets the OnBegin action for a given action in the state machine.
	// @param name The name of the action. 
	// @param pOnBegin Pointer to the function to be called when the action begins.
	///////////////////////////////////////////////////////////////////////////////////
	void StateMachine::SetOnBeginAction(String& name, OnBegin pOnBegin)
	{
		m_actions[name].onBegin = pOnBegin;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// @brief Sets the OnUpdate action for a given action in the state machine.
	// @param name The name of the action.
	// @param pOnUpdate Pointer to the function to be called when the action is updated.
	////////////////////////////////////////////////////////////////////////////////////////
	void StateMachine::SetOnUpdateAction(String& name, OnUpdate pOnUpdate)
	{
		m_actions[name].onUpdate = pOnUpdate;
	}

	///////////////////////////////////////////////////////////////////////////////
	// @brief Sets the OnEnd action for a given action in the state machine.
	// @param name The name of the action.
	// @param pOnEnd Pointer to the function to be called when the action ends.
	///////////////////////////////////////////////////////////////////////////////
	void StateMachine::SetOnEndAction(String& name, OnEnd pOnEnd)
	{
		m_actions[name].onEnd = pOnEnd;
	}

	////////////////////////////////////////////////////
	// @brief Add a transition to the state machine.
	// @param transition The transition to be added.
	////////////////////////////////////////////////////
	void StateMachine::AddTransition(Transition transition)
	{
		m_actions[transition.fromTarget].m_transitions.PushBack(transition);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// @brief Add a transition to the state machine with conditions and target.
	// @param conditions The conditions that must be met for the transition to occur.
	// @param target The target action to transition to when conditions are met.
	////////////////////////////////////////////////////////////////////////////////////
	void StateMachine::AddTransition(Vector<Condition>& conditions, String& fromTarget, String& toTarget)
	{
		AddTransition(Transition(conditions, fromTarget, toTarget));
	}

	///////////////////////////////////////
	// @brief Update the state machine.
	///////////////////////////////////////
	void StateMachine::Update()
	{
		for (auto it = m_actions[m_actualAction].m_transitions.begin(); it != m_actions[m_actualAction].m_transitions.end(); ++it)
		{
			Transition transition = *it;
			for (auto it2 = transition.conditions.begin(); it2 != transition.conditions.end(); ++it2)
			{
				bool transit = true;

				Condition condition = *it2;

				transit = transit && it2->condition();

				if (transit)
				{
					String target = transition.toTarget;
					return Transit(target);
				}
			}


		}
		m_actions[m_actualAction].onUpdate();
	}

	///////////////////////////////////////////////////////////////////////
	// @brief Transits the state machine to a target action.
	// @param target The name of the targeted action to transition to.
	//////////////////////////////////////////////////////////////////////
	void StateMachine::Transit(String target)
	{
		if (m_actions[m_actualAction].onEnd)
		{
			m_actions[m_actualAction].onEnd();
		}
		m_actualAction = target;

		actualAction = m_actualAction;

		if (m_actions[m_actualAction].onBegin)
		{
			m_actions[m_actualAction].onBegin();
		}
	}
}