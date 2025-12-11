#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "define.h"
#include <Engine.h>
#include <Render.h>

using namespace gce;

class CustomScene;
class gce::Scene;

enum SceneName
{
	MAIN_MENU,
	GAME,
	PAUSE_MENU,
	GAMEOVER,
	OPTIONS_MENU
};

class SceneManager
{
public:

	void ChangeScene(SceneName name);
	void Init();
	CustomScene* GetCurrentScene() { return m_vScene[currScene]; }
	CustomScene* GetScene(SceneName name) { return m_vScene[name]; }
	static SceneManager* GetInstance();

protected:
	std::vector<CustomScene*> m_vScene;
	SceneName currScene;

private:
	Scene* m_pScene;
	static SceneManager* p_Instance;
};


#endif // !SCENEMANAGER_HPP_INCLUDED

