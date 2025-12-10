#ifndef LEVELGRID_H_DEFINED
#define LEVELGRID_H_DEFINED
#include "Engine.h"
#include "SceneManager.h"

using namespace gce;

#define MAX_COST (2147483647 / 2)

struct Data
{
    Vector3f32 position;
    bool isAvailable;

    float32 Distance(Data const& other)
    {
        return Sqrt(Pow(position.x - other.position.x, 2) + Pow(position.y - other.position.y, 2) + Pow(position.z - other.position.z, 2));
    }
};

struct Node
{
    Data* data = nullptr;
    bool isVisited = false;
    Node* pCameFrom = nullptr;

    int cost = MAX_COST;
    int targetDistance = MAX_COST;

    bool operator()(Node const* a, Node const* b) const
    {
        int totalCostA = a->cost + a->targetDistance;
        int totalCostB = b->cost + b->targetDistance;

        if (totalCostA == totalCostB)
            return a->cost < b->cost;

        return totalCostA > totalCostB;
    }
};

class LevelGrid 
{
public:
    void Init(SceneName scene, std::pair<Vector3f32, Vector3f32>const& mapProperties, Vector3f32 tileSize);
    void Reset();
    Node* AStar(Node* pStart, Node* pEnd);

private:
    std::pair<Vector3f32, Vector3f32> m_mapProperties;
    Vector3f32 m_tileSize;
    
    Vector<Node> m_vNode;
    Vector<Vector<Vector<Data>>> m_vData;
    
    bool CalculateAvailability(Vector3f32 position, Vector<GameObject*>& objs);

};

#endif
