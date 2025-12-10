#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "GameTransform.h"
#include "Maths/Physics.h"
#include "MeshRenderer.h"
#include "Geometry.h"

namespace gce {

#define BOX_DEFAULT_VALUE .5f

void BoxCollider::Init()
{
	m_pOwner->pCollisionEvents = &collisionEnterEvent;
	m_pOwner->transform.dirtyEvent.AddListener(&BoxCollider::UpdateColliderComponent, this);

	if (!GetOwner().HasComponent<gce::MeshRenderer>())
	{
		m_worldBox.min = { -BOX_DEFAULT_VALUE, -BOX_DEFAULT_VALUE, -BOX_DEFAULT_VALUE };
		m_worldBox.max = { BOX_DEFAULT_VALUE, BOX_DEFAULT_VALUE, BOX_DEFAULT_VALUE };
		localOBBPoints = Physics::GetListFromMinMax(m_worldBox.min, m_worldBox.max);

		Physics::UpdateBoundingBox(m_worldBox, GetOwner().transform.m_matrix, localScale );
		return;
	}

	Geometry* geo = GetOwner().GetComponent<gce::MeshRenderer>()->pGeometry;
	GameTransform& transform = GetOwner().transform;
	m_worldBox.min = geo->min * localScale * transform.GetWorldScale();
	m_worldBox.max = geo->max * localScale * transform.GetWorldScale();
	localOBBPoints = Physics::GetListFromMinMax(m_worldBox.min, m_worldBox.max);

	Physics::UpdateBoundingBox(m_worldBox, GetOwner().transform.m_matrix, localScale);
}

void BoxCollider::UpdateColliderComponent( uint8 const dirty )
{
	GameTransform& transform = GetOwner().transform;

	if ( GetOwner().HasComponent<gce::MeshRenderer>() )
	{
		Geometry* const pGeometry = GetOwner().GetComponent<gce::MeshRenderer>()->pGeometry;
		m_worldBox.min = localScale * transform.GetWorldScale() * pGeometry->min;
		m_worldBox.max = localScale * transform.GetWorldScale() * pGeometry->max;
	}
	else
	{
		m_worldBox.min = localScale * transform.GetWorldScale() * -BOX_DEFAULT_VALUE;
		m_worldBox.max = localScale * transform.GetWorldScale() * BOX_DEFAULT_VALUE;
	}

	m_worldBox.center = transform.GetWorldPosition() + m_localOffSet * transform.GetWorldScale();
	m_worldBox.aabb.center = m_worldBox.center;

	//Apply RotMatrix To center 
	if (m_localOffSet != Vector3f32{ 0,0,0 })
	{
		Physics::ScaleNRotateVect3(m_worldBox.center, transform.m_matrix);
	}

	m_worldBox.axisX = Physics::ScaleNRotateVect3(Vector3f32{1,0,0}, transform.m_matrix).Normalize();
	m_worldBox.axisY = Physics::ScaleNRotateVect3(Vector3f32{0,1,0}, transform.m_matrix).Normalize();
	m_worldBox.axisZ = Physics::ScaleNRotateVect3(Vector3f32{0,0,1}, transform.m_matrix).Normalize();

	Physics::UpdateBoundingBox(m_worldBox, transform.m_matrix, localScale);

	//Apply RotMatrix To center Point
	if (m_localOffSet != Vector3f32{ 0,0,0 })
	{
		Physics::ScaleNRotateVect3(m_worldBox.center, transform.m_matrix);
	}
}

#undef BOX_DEFAULT_VALUE
}