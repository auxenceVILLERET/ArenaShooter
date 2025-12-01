#include "pch.h"
#include "Scene.h"

#include <ranges>

#include "GameObject.h"
#include "GameManager.h"

namespace gce {


Scene& Scene::Create()
{
    Scene* const pNew = new Scene();
    GameManager::s_pInstance->m_scenes.PushBack( pNew );
    return *pNew;
}

void Scene::UpdateMatrix()
{
    for (GameObject* const pGameObject : m_gameObjects | std::views::values)
    {
        if (pGameObject->IsActive() == false) continue;

        pGameObject->transform.UpdateMatrix();
    }
}

}