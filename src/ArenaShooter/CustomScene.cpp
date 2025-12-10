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
	m_vObject.PushBack(&object);
	object.SetActive(IsActive);
	return object;
}

Vector<GameObject*>& CustomScene::GetObjects()
{
	CleanVector();
	return m_vObject;
}

void CustomScene::SetActive()
{
	for (int i = 0; i < m_vObject.Size(); i++)
	{
		m_vObject[i]->SetActive(true);
	}
}

void CustomScene::SetInactive()
{
	for (int i = 0; i < m_vObject.Size(); i++)
	{
		m_vObject[i]->SetActive(false);
	}
}

void CustomScene::CleanVector()
{
	for (int i = 0; i < m_vObject.Size(); i++)
	{
		if (m_vObject[i] == nullptr)
			m_vObject.Erase(m_vObject.begin() + i);
	}
}
