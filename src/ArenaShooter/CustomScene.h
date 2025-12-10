#ifndef CUSTOMSCENE_H_INCLUDED
#define CUSTOMSCENE_H_INCLUDED

#include "define.h"
#include "Scene.h"

using namespace gce;

class gce::GameObject;

class CustomScene
{
public:
	CustomScene(Scene* pScene);
	GameObject& AddObject();
	void SetActive();
	void SetInactive();

protected:
	
private:
	std::vector<GameObject> m_vObject;
	Scene* m_pScene;

};


#endif

