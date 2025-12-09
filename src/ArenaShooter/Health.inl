#ifndef HEALTH_CPP_INCLUDED
#define HEALTH_CPP_INCLUDED
#include "Health.h"

template<typename T>
Health<T>::Health(T max)
{
	m_MaxHealth = max;
	m_Health = max;
}

template<typename T>
T Health<T>::GetHealth()
{
	return T(m_Health);
}

template<typename T>
bool Health<T>::GetIsAlive()
{
	return m_IsAlive;
}

template<typename T>
void Health<T>::SetIsAlive(bool newState)
{
	m_IsAlive = newState;
}

template<typename T>
void Health<T>::TakeDamage(T ammount)
{
	m_Health -= ammount;
	if (m_Health <= 0.f)
	{
		m_Health = 0.f;
	}
}

template<typename T>
void Health<T>::Heal(T ammount)
{
	m_Health += ammount;
	if (m_Health >= m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}


#endif // !HEALTH_CPP_INCLUDED
