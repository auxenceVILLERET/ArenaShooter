#ifndef LEVELGRID_CPP_DEFINED
#define LEVELGRID_CPP_DEFINED
#include "LevelGrid.h"
#include "Maths/MathsFunctions.hpp"

#include "CustomScene.h"

void LevelGrid::Init(SceneName scene, std::pair<Vector3f32, Vector3f32> const& mapProperties, Vector3f32 tileSize)
{
    m_mapProperties = mapProperties;
    m_tileSize = tileSize;
    Vector<GameObject*>& vObjs = SceneManager::GetInstance()->GetScene(scene)->GetObjects();
    
    int tileCountX = mapProperties.first.x / tileSize.x + 1.0f;
    int tileCountY = mapProperties.first.y / tileSize.y + 1.0f;
    int tileCountZ = mapProperties.first.z / tileSize.z + 1.0f;

    Vector3f32 startTile = {mapProperties.first.x - mapProperties.second.x * 0.5f + tileSize.x / 2.0f,
                            mapProperties.first.y - mapProperties.second.y * 0.5f + tileSize.y / 2.0f,
                            mapProperties.first.z - mapProperties.second.z * 0.5f + tileSize.z / 2.0f};
    
    for (int z = 0; z < tileCountZ; z++)
    {
        Vector<Vector<Data>> tempY;
        for (int y = 0; y < tileCountY; y++)
        {
            Vector<Data> tempX;
            for (int x = 0; x < tileCountX; x++)
            {
                Data newTile;
                newTile.position = {startTile.x + x * tileSize.x,
                                         startTile.y + y * tileSize.y,
                                         startTile.z + z * tileSize.z};
                newTile.isAvailable = CalculateAvailability(newTile.position, vObjs);
                tempX.PushBack(newTile);
            }
            tempY.PushBack(tempX);
        }
        m_vData.PushBack(tempY);
    }
}

bool LevelGrid::CalculateAvailability(Vector3f32 position, Vector<GameObject*>& objs)
{
    GameObject& temp = SceneManager::GetInstance()->GetCurrentScene()->AddObject();
    BoxCollider* b = temp.AddComponent<BoxCollider>();

    for (GameObject* obj : objs)
    {
        BoxCollider* oB = obj->GetComponent<BoxCollider>();
        SphereCollider* oS = obj->GetComponent<SphereCollider>();

        if (oB == nullptr && oS == nullptr)
            continue;

        if (oB != nullptr)
        {
            CollideResult intersects = Physics::IntersectBoxBox(b->m_worldBox, oB->m_worldBox);
            if (intersects.isColliding)
                return false;
        }

        if (oS != nullptr)
        {
            CollideResult intersects = Physics::IntersectSphereBox(oS->m_worldSphere, b->m_worldBox);
            if (intersects.isColliding)
                return false;
        }
    }

    temp.Destroy();
    return true;
}

void LevelGrid::Reset()
{
}

Node* LevelGrid::AStar(Node* pStart, Node* pEnd)
{
}


#endif
