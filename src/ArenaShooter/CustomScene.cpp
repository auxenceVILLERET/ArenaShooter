#include "CustomScene.h"
#include "GameObject.h"
#include "SceneManager.h"

CustomScene::CustomScene(Scene* pScene)
{
	m_pScene = pScene;
}

CustomScene::~CustomScene()
{

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

template <typename T>
Vector<T*> CustomScene::GetAllScripts()
{
	Vector<T*> vScripts;
	for (GameObject* pObject : m_vObject)
	{
		T* pScript = pObject->GetScript<T>();
		if (pScript != nullptr)
			vScripts.PushBack(pScript);
	}
	return vScripts;
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

void CustomScene::Init()
{
	isInit = true;
}


void CustomScene::Start()
{
	if (isInit == false)
		Init();
}

void CustomScene::CleanVector()
{
	for (int i = 0; i < m_vObject.Size(); i++)
	{
		if (m_vObject[i] == nullptr)
			m_vObject.Erase(m_vObject.begin() + i);
	}
}

void CustomScene::Empty(int indexStart)
{
	CleanVector();
	int size = m_vObject.Size() - indexStart;
	for (int i = 0; i < size; i++)
	{
		m_vObject[indexStart]->SetActive(true);
		m_vObject.Back()->Destroy();
		m_vObject.PopBack();
	}
}
