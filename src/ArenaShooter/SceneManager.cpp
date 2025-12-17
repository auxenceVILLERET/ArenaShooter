#include "SceneManager.h"
#include "CustomScene.h"
#include "Scene.h"
#include "GameManager.h"
#include "GameMenu.hpp"
#include "MainMenu.hpp"
#include "OptionsMenu.hpp"
#include "GameoverMenu.hpp"

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

	m_vScene.PushBack(new MainMenu(m_pScene));
	m_vScene.PushBack(new GameScene(m_pScene));
	m_vScene.PushBack(new OptionMenu(m_pScene));
	m_vScene.PushBack(new GameoverMenu(m_pScene));

	for (int i = 0; i < m_vScene.Size(); i++)
	{
		m_vScene[i]->Init();
	}
}

void SceneManager::ChangeScene(SceneName name)
{
	GetScene(currScene)->SetInactive();
	GetScene(currScene)->IsActive = false;
	currScene = name;
	GetScene(currScene)->SetActive();
	GetScene(currScene)->IsActive = true;
	GetScene(currScene)->Start();
}
