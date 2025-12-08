#ifndef HEALTH_H_INCLUDED
#define HEALTH_H_INCLUDED

template<typename T>
class Health
{
public:
	Health(T max);
	void TakeDamage(T ammount);
	void Heal(T ammount);
	T GetHealth();

	bool GetIsAlive();
	void SetIsAlive(bool newState);

protected:

private:
	T m_MaxHealth;
	T m_Health;
	bool m_IsAlive = true;
};

#include "Health.inl"
#endif // !HEALTH_H_INCLUDED

