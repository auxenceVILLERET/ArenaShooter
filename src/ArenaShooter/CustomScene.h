#ifndef CUSTOMSCENE_H_INCLUDED
#define CUSTOMSCENE_H_INCLUDED

#include "define.h"

using namespace gce;

class gce::GameObject;

class CustomScene
{
public:
	GameObject& AddObject();
	void SetActive();
	void SetInactive();

protected:
	
private:
	std::vector<GameObject> m_vObject;
};


#endif

