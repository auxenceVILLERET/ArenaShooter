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
	~CustomScene();
	GameObject& AddObject();
	Vector<GameObject*>& GetObjects();
	void SetActive();
	void SetInactive();

	virtual void Start();
	
protected:
	virtual void Init();
	
private:
	Vector<GameObject*> m_vObject;
	Scene* m_pScene;
	bool IsActive = false;
	bool isInit = false;

	void CleanVector();
	
	friend class SceneManager;
};


#endif

