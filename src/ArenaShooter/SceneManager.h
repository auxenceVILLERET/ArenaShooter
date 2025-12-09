#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "define.h"

class CustomScene;

class SceneManager
{
public:

	void ChangeScene(String name);
	CustomScene* GetScene() { return m_currentScene; };

protected:
	std::vector<CustomScene> m_Scene;
	CustomScene* m_currentScene;

private:

};


#endif // !SCENEMANAGER_HPP_INCLUDED

