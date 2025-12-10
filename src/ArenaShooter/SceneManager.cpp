#include "SceneManager.h"
#include "CustomScene.h"
#include "Scene.h"
#include "GameManager.h"

SceneManager* SceneManager::p_Instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (p_Instance == nullptr)
	{
		p_Instance = new SceneManager();
	}
	return p_Instance;
}

void SceneManager::Init()
{
	GameManager::Create();
	m_pScene = &Scene::Create();

	for (int i = 0; i < 5; i++)
	{
		m_vScene.push_back(new CustomScene(m_pScene));
	}
}

void SceneManager::ChangeScene(SceneName name)
{
	GetScene(currScene)->SetInactive();
	GetScene(currScene)->IsActive = false;
	currScene = name;
	GetScene(currScene)->SetActive();
	GetScene(currScene)->IsActive = true;
}
