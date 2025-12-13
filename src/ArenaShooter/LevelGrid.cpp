#ifndef LEVELGRID_CPP_DEFINED
#define LEVELGRID_CPP_DEFINED
#include "LevelGrid.h"

#include <queue>

#include "Maths/MathsFunctions.hpp"

#include "CustomScene.h"

Node* LevelGrid::GetNode(Vector3i32 const& pos)
{
    for (int i = 0; i < m_vNodes.Size(); i++)
    {
        if (m_vNodes[i].data->gridPosition.x == pos.x && m_vNodes[i].data->gridPosition.y == pos.y && m_vNodes[i].data->gridPosition.z == pos.z)
            return &m_vNodes[i];
    }
    return nullptr;
}

Vector3i32 LevelGrid::GetTilePosition(Vector3f32 const& pos)
{
    Vector3f32 anchorPoint = {m_mapProperties.first.x - m_mapProperties.second.x * 0.5f,
                              m_mapProperties.first.y - m_mapProperties.second.y * 0.5f,
                              m_mapProperties.first.z - m_mapProperties.second.z * 0.5f};
    
    Vector3f32 newPos = { pos.x - anchorPoint.x, pos.y - anchorPoint.y, pos.z - anchorPoint.z };
    newPos /= m_tileSize;
    
    Vector3i32 result(RoundToInt(newPos.x), RoundToInt(newPos.y), RoundToInt(newPos.z));

    return result;
}

void LevelGrid::Init(SceneName scene, std::pair<Vector3f32, Vector3f32> const& mapProperties, Vector3f32 tileSize)
{
    m_mapProperties = mapProperties;
    m_tileSize = tileSize;
    Vector<GameObject*>& vObjs = SceneManager::GetInstance()->GetScene(scene)->GetObjects();
    
    int tileCountX = mapProperties.second.x / tileSize.x + 1.0f;
    int tileCountY = mapProperties.second.y / tileSize.y + 1.0f;
    int tileCountZ = mapProperties.second.z / tileSize.z + 1.0f;

    Vector3f32 startTile = {mapProperties.first.x - mapProperties.second.x * 0.5f + tileSize.x * 0.5f,
                            mapProperties.first.y - mapProperties.second.y * 0.5f + tileSize.y * 0.5f,
                            mapProperties.first.z - mapProperties.second.z * 0.5f + tileSize.z * 0.5f};

    tempCollider = &SceneManager::GetInstance()->GetCurrentScene()->AddObject();
    tempCollider->AddComponent<BoxCollider>();
    tempCollider->transform.SetWorldScale(m_tileSize * 0.5f);
    
    for (int y = 0; y < tileCountY; y++)
    {
        Vector<Vector<Data>> tempZ;
        for (int z = 0; z < tileCountZ; z++)
        {
            Vector<Data> tempX;
            for (int x = 0; x < tileCountX; x++)
            {
                
                Data newTile;
                newTile.gridPosition = {x, y, z};
                newTile.worldPosition =
                    {
                    startTile.x + x * tileSize.x,
                    startTile.y + y * tileSize.y,
                    startTile.z + z * tileSize.z
                    };
                newTile.isAvailable = CalculateAvailability(newTile.worldPosition, vObjs);
                tempX.PushBack(newTile);
            }
            tempZ.PushBack(tempX);
        }
        m_vData.PushBack(tempZ);
    }

    tempCollider->Destroy();

    CalculateNodes();
}

bool LevelGrid::CalculateAvailability(Vector3f32 position, Vector<GameObject*>& objs)
{
    tempCollider->transform.SetWorldPosition(position);
    tempCollider->transform.SetWorldScale(m_tileSize * 0.5f);
    tempCollider->transform.UpdateMatrix();
    BoxCollider* b = tempCollider->GetComponent<BoxCollider>();

    if (b == nullptr) return false;
    
    for (GameObject* obj : objs)
    {
        BoxCollider* oB = obj->GetComponent<BoxCollider>();
        SphereCollider* oS = obj->GetComponent<SphereCollider>();
        obj->transform.UpdateMatrix();

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
    
    return true;
}

Vector<Vector3i32> LevelGrid::GetNeighbours(Vector3i32 pos, Vector3i32 minPos, Vector3i32 maxPos)
{
    Vector<Vector3i32> neighbours;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            for (int k = -1; k <= 1; k++)
            {
                if (j == 0 && i == 0 && k == 0)                                             continue;
                if (pos.y + j < minPos.y || pos.x + i < minPos.x || pos.z + k < minPos.z)   continue;
                if (pos.y + j > maxPos.y || pos.x + i > maxPos.x || pos.z + k > maxPos.z)   continue;


                neighbours.PushBack({ pos.x + i, pos.y + j, pos.z + k });   
            }
        }
    }
    return neighbours;
}

void LevelGrid::CalculateNodes()
{
    m_vNodes.Clear();
    
    m_WIDTH = m_vData.Front().Front().Size();
    m_LENGTH = m_vData.Front().Size();
    m_HEIGHT = m_vData.Size();

    for (int i = 0; i < m_HEIGHT; i++)
    {
        for (int j = 0; j < m_LENGTH; j++)
        {
            for (int k = 0; k < m_WIDTH; k++)
            {
                Node tempNode;
                tempNode.data = &m_vData[i][j][k];
                m_vNodes.PushBack(tempNode);   
            }
        }
    }

    for (int i = 0; i < m_vNodes.Size(); i++)
    {
        Node* n = &m_vNodes[i];
        n->vNeighbours.Clear();
        Vector<Vector3i32> neighbours = GetNeighbours(n->data->gridPosition, {0, 0, 0}, {m_WIDTH - 1, m_HEIGHT - 1, m_LENGTH - 1});

        for (int i = 0; i < neighbours.Size(); i++)
        {
            Vector3i32 p = neighbours[i];
            Data* neig = GetNode(p)->data;

            if (neig->isAvailable == false)
                continue;
            if (IsEqual(n->data->Distance(*neig), Sqrt(3)))
            {
                Vector3i32 centerPos = n->data->gridPosition;
                Data* n1 = &m_vData[centerPos.y][p.z][p.x];
                Data* n2 = &m_vData[p.y][centerPos.z][p.x];
                Data* n3 = &m_vData[p.y][p.z][centerPos.x];
                
                if (n1->isAvailable == false || n2->isAvailable == false || n3->isAvailable == false)
                    continue;
            }

            Node* nNeig = GetNode(neig->gridPosition);
            
            n->vNeighbours.PushBack(nNeig);
        }
    }
}

void LevelGrid::Reset()
{
    for (int i = 0; i < m_vNodes.Size(); i++)
    {
        m_vNodes[i].cost = MAX_COST;
        m_vNodes[i].targetDistance = MAX_COST;
        m_vNodes[i].pCameFrom = nullptr;
        m_vNodes[i].isVisited = false;
    }
}

Node* LevelGrid::AStar(Node* pStart, Node* pEnd, Enemy* pAgent)
{
    std::priority_queue<Node*, std::vector<Node*>, Node> queue;
    pStart->cost = 0;
    queue.push(pStart);

    while (!queue.empty())
    {
        Node* curr = queue.top();
        queue.pop();

        if (curr->isVisited == true)
            continue;

        if (curr == pEnd)
            return pEnd;

        curr->isVisited = true;

        Vector<Node*> neighbours = curr->vNeighbours;
        for (int i = 0; i < neighbours.Size(); i++)
        {
            Node* neighbour = neighbours[i];
            if (neighbour->isVisited)
                continue;
            if (neighbour->data->pOccupyingAgent != nullptr && neighbour->data->pOccupyingAgent != pAgent)
                continue;

            neighbour->targetDistance = neighbour->data->Distance(*pEnd->data);
            
            float32 newCost = curr->cost + curr->data->Distance(*neighbour->data);
            if (newCost < neighbour->cost)
            {
                neighbour->cost = newCost;
                neighbour->pCameFrom = curr;
                queue.push(neighbour);
            }
        }
    }

    return nullptr;
}

#endif
