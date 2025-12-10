#include "CustomScene.h"
#include "GameObject.h"
#include "SceneManager.h"

CustomScene::CustomScene(Scene* pScene)
{
	m_pScene = pScene;
}

GameObject& CustomScene::AddObject()
{
	GameObject& object = GameObject::Create(*m_pScene);
	m_vObject.push_back(object);
}

void CustomScene::SetActive()
{
	for (int i = 0; i < m_vObject.size(); i++)
	{
		m_vObject[i].SetActive(true);
	}
}

void CustomScene::SetInactive()
{
	for (int i = 0; i < m_vObject.size(); i++)
	{
		m_vObject[i].SetActive(false);
	}
}
