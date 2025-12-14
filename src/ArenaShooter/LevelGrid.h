#ifndef LEVELGRID_H_DEFINED
#define LEVELGRID_H_DEFINED
#include "Engine.h"
#include "MapLoader.hpp"
#include "SceneManager.h"

using namespace gce;

struct Enemy;
struct MapProperties;

#define MAX_COST (2147483647 / 2)

struct Data
{
    Vector3i32 gridPosition;
    Vector3f32 worldPosition;
    bool isAvailable;
    
    Enemy* pOccupyingAgent = nullptr;
    
    float32 Distance(Data const& other)
    {
        return Sqrt(Pow(gridPosition.x - other.gridPosition.x, 2) + Pow(gridPosition.y - other.gridPosition.y, 2) + Pow(gridPosition.z - other.gridPosition.z, 2));
    }
};

struct Node
{
    Data* data = nullptr;
    bool isVisited = false;
    Node* pCameFrom = nullptr;

    float32 cost = (float32)MAX_COST;
    float32 targetDistance = (float32)MAX_COST;

    Vector<Node*> vNeighbours;

    bool operator()(Node const* a, Node const* b) const
    {
        float32 totalCostA = a->cost + a->targetDistance;
        float32 totalCostB = b->cost + b->targetDistance;

        if (totalCostA == totalCostB)
            return a->cost < b->cost;

        return totalCostA > totalCostB;
    }
};

namespace gce
{
    class LevelGrid 
    {
    public:
        void Init(SceneName scene, MapProperties const& mapProperties, Vector3f32 tileSize);
        void Reset();
        Node* AStar(Node* pStart, Node* pEnd, Enemy* pAgent);
        void CalculateNodes();
        
        Node* GetNode(Vector3i32 const& pos);
        Vector3i32 GetTilePosition(Vector3f32 const& pos);

    private:
        int32 m_WIDTH  = 0;
        int32 m_LENGTH = 0;
        int32 m_HEIGHT = 0;
        
        MapProperties m_mapProperties;
        Vector3f32 m_tileSize;
    
        Vector<Node> m_vNodes;
        Vector<Vector<Vector<Data>>> m_vData;
        GameObject* tempCollider;
        
        bool CalculateAvailability(Vector3f32 position, Vector<GameObject*>& objs);
        Vector<Vector3i32> GetNeighbours(Vector3i32 pos, Vector3i32 minPos, Vector3i32 maxPos);

    };   
}

#endif
