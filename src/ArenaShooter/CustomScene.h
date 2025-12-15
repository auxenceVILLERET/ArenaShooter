#ifndef CUSTOMSCENE_H_INCLUDED
#define CUSTOMSCENE_H_INCLUDED

#include "define.h"
#include "Scene.h"
#include "Containers/Vector.hpp"

using namespace gce;

class gce::GameObject;

class CustomScene
{
public:
	CustomScene(Scene* pScene);
	GameObject& AddObject();
	Vector<GameObject*>& GetObjects();
	void SetActive();
	void SetInactive();
	
protected:
	
private:
	Vector<GameObject*> m_vObject;
	Scene* m_pScene;
	bool IsActive = false;

	void CleanVector();
	
	friend class SceneManager;
};


#endif

