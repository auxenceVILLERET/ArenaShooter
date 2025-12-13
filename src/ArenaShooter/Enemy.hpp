#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Health.h"
#include "Projectile.hpp"
#include "LevelGrid.h"

struct Target 
{
	Vector3f32 position;
	float32 distance = 0.0f;
	bool isSet = false;
	bool isReached = false;
};

struct Path
{
	bool isLoop = false;
	int8 index = 0;
	Vector<Vector3i32> vPositions;
};

using namespace gce;

DECLARE_SCRIPT(Enemy, ScriptFlag::Awake | ScriptFlag::Update | ScriptFlag::CollisionEnter)

Health<float>* m_Hp;
GameObject* m_pPlayer;

Target m_target;
float32 m_speed = 0.0f;
Vector3f32 m_direction;
Vector<Path> m_vPaths;

LevelGrid* m_pLevelGrid = nullptr;

void Awake() override
{
	
}

void Update() override
{
	float32 dt = GameManager::DeltaTime();
	float32 distance = dt * m_speed;
	if (distance > m_target.distance)
		distance = m_target.distance;
	Vector3f32 translation = m_direction * distance;
	m_pOwner->transform.WorldTranslate(translation);
	
	if (m_target.isSet) 
	{
		m_target.distance = m_pOwner->transform.GetWorldPosition().Distance(m_target.position);

		if (m_target.distance <= 0.001f)
		{
			m_pOwner->transform.SetWorldPosition(m_target.position);
			m_direction = Vector3f32({ 0.f, 0.f, 0.f });
			m_target.isSet = false;
			m_target.isReached = true;
		}
	}

	if (m_pLevelGrid == nullptr)
		return;

	if (m_vPaths.Empty()) return;
	if (m_target.isSet == false)
		SetTarget();
	
}

void SetPlayer(GameObject* player)
{
	m_pPlayer = player;
}

virtual void Shoot()
{

}

void SetGrid(LevelGrid* pGrid) { m_pLevelGrid = pGrid; }

void CollisionEnter(GameObject* pOther) override
{
	if(pOther->GetScript<BulletRifle>())
	{
		m_Hp->TakeDamage(pOther->GetScript<BulletRifle>()->GetDmgBullet());
		std::cout << m_Hp->GetHealth() << std::endl;
	}
	if (pOther->GetScript<BulletShotgun>())
	{
		m_Hp->TakeDamage(pOther->GetScript<BulletShotgun>()->GetDmgBullet());
		std::cout << m_Hp->GetHealth() << std::endl;
	}
	if (pOther->GetScript<BulletHandgun>())
	{
		m_Hp->TakeDamage(pOther->GetScript<BulletHandgun>()->GetDmgBullet());
		std::cout << m_Hp->GetHealth() << std::endl;
	}
}

void GoToPosition(Vector3f32 const& pos, float32 speed)
{
	Vector3f32 position = m_pOwner->transform.GetWorldPosition();
	Vector3f32 direction = Vector3f32(pos.x - position.x, pos.y - position.y, pos.z - position.z);

	float32 norm = direction.Norm();
	if (norm == 0.0f) return;
	
	direction.SelfNormalize();

	m_direction = direction;

	if (speed > 0.0f)
		m_speed = speed;

	m_target.position = pos;
	m_target.distance = position.Distance(pos);
	m_target.isSet = true;
	m_target.isReached = false;
	
	Vector3f32 eye = m_pOwner->transform.GetWorldPosition();
	Vector3f32 target = m_pPlayer->transform.GetWorldPosition();
	Vector3f32 dir = (target - eye).Normalize();
	float yaw = atan2(dir.x, dir.z);
	float pitch = asin(-dir.y);
	Quaternion q = Quaternion::RotationEuler(pitch, yaw, 0);
	m_pOwner->transform.SetLocalRotation(q);
}

void SetTarget()
{
	if (m_target.isSet == true) return;
	if (m_vPaths.Empty()) return;
	Path& currentPath = m_vPaths.Front();

	if (m_pLevelGrid == nullptr) return;
	
	if (currentPath.index < currentPath.vPositions.Size())
	{
		Vector3i32 p = currentPath.vPositions[currentPath.index];
		Vector3f32 position = m_pLevelGrid->GetNode(p)->data->worldPosition;
		GoToPosition(position, m_speed);
		currentPath.index++;
		std::cout << position.x << "|" << position.y << "|" << position.z << std::endl;
	}
	else
		m_vPaths.Erase(m_vPaths.begin());
}

Node* GetNextNode()
{
	LevelGrid* grid = m_pLevelGrid;
	if (grid == nullptr) return nullptr;
	Node* node = grid->GetNode(grid->GetTilePosition(m_pOwner->transform.GetWorldPosition()));
	
	Vector3f32 worldPos = m_pOwner->transform.GetWorldPosition();
	Vector3f32 scale = m_pOwner->transform.GetWorldScale();
	Vector3f32 nextTilePos = worldPos + Vector3f32(m_direction.x * scale.x, m_direction.y * scale.y, m_direction.z * scale.z);
	Vector3i32 tileP = grid->GetTilePosition(nextTilePos);

	Node* nextNode = grid->GetNode(tileP);
	if (node == nextNode) return nullptr;

	return nextNode;
}

void SetPath(Vector3f32 target)
{
	LevelGrid* grid = m_pLevelGrid;
	if (grid == nullptr) return;;
	
	Node* nEnd = grid->GetNode(grid->GetTilePosition(target));
	Node* nSelf = grid->GetNode(grid->GetTilePosition(m_pOwner->transform.GetWorldPosition()));

	if (nSelf == nullptr) return ;
	if (nEnd == nullptr) return ;

	Vector3f32 targetDir = target - m_pOwner->transform.GetWorldPosition();
	float32 distance = targetDir.Norm();
	targetDir.SelfNormalize();
	
	Path path;
	//
	Vector<Vector2f32> dirs;
	dirs.PushBack(Vector2f32(0.0f, 0.0f));
	dirs.PushBack(Vector2f32(1.0f, 0.0f));
	dirs.PushBack(Vector2f32(-1.0f, -1.0f));
	dirs.PushBack(Vector2f32(-1.0f, 1.0f));
	
	Ray ray;
	bool blocked = false;
	
	for (Vector2f32 dir : dirs)
	{
		ray.origin = m_pOwner->transform.GetWorldPosition();
		ray.direction = targetDir;
	
		ray.origin += m_pOwner->transform.GetWorldRight() * dir.x * m_pOwner->transform.GetWorldScale().x * 0.5f;
		ray.origin += m_pOwner->transform.GetWorldUp() * dir.y * m_pOwner->transform.GetWorldScale().y * 0.5f;
	
		RaycastHit hitInfo;
		bool hit = PhysicSystem::IntersectRay(ray, hitInfo, distance);
	
		if (hit && hitInfo.pGameObject != nullptr && hitInfo.pGameObject->GetName() != "Player" &&  hitInfo.pGameObject != m_pOwner)
		{
			blocked = true;
			break;
		}
	}
	
	if (blocked == false && m_vPaths.Empty())
	{
		ResetPath();
		GoToPosition(target, m_speed);
		return;
	}
	
	Node* nResult = grid->AStar(nSelf, nEnd, this);
	if (nResult == nullptr)
		return;

	while (nResult != nullptr)
	{
		Vector3i32 p = nResult->data->gridPosition;
		path.vPositions.Insert(path.vPositions.begin(), p);
		nResult = nResult->pCameFrom;
	}

	path.vPositions.Erase(path.vPositions.begin());
	if (m_vPaths.Empty() == false)
	{
		if (path.vPositions.Back() == m_vPaths.Front().vPositions.Back())
			return;
	}

	grid->Reset();

	Path copy = path;
	
	if (path.vPositions.Size() > 1)
	{
		Vector3f32 dir = { 0, 0, 0 };
		int i = 1;
		while (i < path.vPositions.Size())
		{
			Vector3f32 a = {(float32)path.vPositions[i].x, (float32)path.vPositions[i].y, (float32)path.vPositions[i].z};
			Vector3f32 b = {(float32)path.vPositions[i - 1].x, (float32)path.vPositions[i - 1].y, (float32)path.vPositions[i - 1].z};

			Vector3f32 newDir = a - b;

			if (i > 1)
			{
				Vector3f32 prevDir = b - Vector3f32((float32)path.vPositions[i - 2].x, (float32)path.vPositions[i - 2].y, (float32)path.vPositions[i - 2].z);

				// check if direction is same
				if (newDir == prevDir)
				{
					path.vPositions.Erase(path.vPositions.begin() + i - 1);
					continue; // do not increment i, because indices shifted left
				}
			}

			i++;
		}
	}

	ResetPath();
	if (path.vPositions.Empty() == false)
		m_vPaths.PushBack(path);
}

void ResetPath()
{
	m_vPaths.Clear();
	m_target.isSet = false;
}


END_SCRIPT


#endif
