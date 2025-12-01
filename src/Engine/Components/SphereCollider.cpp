#include "pch.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "GameTransform.h"
#include "MeshRenderer.h"
#include "Geometry.h"
#include "Maths/Matrix.h"
#include "Maths/Vector3.h"

namespace gce {

#define SPHERE_DEFAULT_VALUE .5f

void SphereCollider::Init()
{
	m_pOwner->pCollisionEvents = &collisionEnterEvent;
	m_pOwner->transform.dirtyEvent.AddListener( &SphereCollider::UpdateColliderComponent, this );
	
	if (!GetOwner().HasComponent<gce::MeshRenderer>())
	{
		localRay = SPHERE_DEFAULT_VALUE;
		return;
	}
	Geometry* geo = GetOwner().GetComponent<gce::MeshRenderer>()->pGeometry;

	localRay = (geo->max.x - geo->min.x) * .5f;
	if (float32 value = (geo->max.y - geo->min.y) * .5f; value > localRay) localRay = value;
	if (float32 value = (geo->max.z - geo->min.z) * .5f; value > localRay) localRay = value;
}

void SphereCollider::UpdateColliderComponent( uint8 const dirty )
{
	GameTransform& transform = m_pOwner->transform;

	m_worldSphere.center = transform.GetWorldPosition() + m_localOffSet * transform.GetWorldScale();

	//Apply RotMatrix To center Point
	if (m_localOffSet != Vector3f32{0,0,0})
	{
		Matrix scaleRot = transform.m_matrix;
		scaleRot.SetValue(3, 0, 0);
		scaleRot.SetValue(3, 1, 0);
		scaleRot.SetValue(3, 2, 0);

		Matrix localPos = Matrix::Translation(m_worldSphere.center);

		localPos = localPos * scaleRot;

		m_worldSphere.center.x = localPos.GetValue(3, 0);
		m_worldSphere.center.y = localPos.GetValue(3, 1);
		m_worldSphere.center.z = localPos.GetValue(3, 2);
	}

	const Vector3f32& scale = transform.GetWorldScale();

	float sphereRayScale = scale.x;
	if (scale.y > sphereRayScale) sphereRayScale = scale.y;
	if (scale.z > sphereRayScale) sphereRayScale = scale.z;

	m_worldSphere.ray = localRay * localScale * sphereRayScale;

	m_worldSphere.center = transform.GetWorldPosition() + m_localOffSet * transform.GetWorldScale(); // QUAND EVENT

	//Apply RotMatrix To center Point
	if (m_localOffSet != Vector3f32{ 0,0,0 })
	{
		Matrix scaleRot = transform.m_matrix;
		scaleRot.SetValue(3, 0, 0);
		scaleRot.SetValue(3, 1, 0);
		scaleRot.SetValue(3, 2, 0);

		Matrix localPos = Matrix::Translation(m_worldSphere.center);

		localPos = localPos * scaleRot;

		m_worldSphere.center.x = localPos.GetValue(3, 0);
		m_worldSphere.center.y = localPos.GetValue(3, 1);
		m_worldSphere.center.z = localPos.GetValue(3, 2);
	}
}

/////////////////////////////////////
/// @brief Check if the ray intersects with SphereColliders
/// @param ray Ray to intersect.
/// @param hitInfo Structure to store the hit information.
/// @param maxDistance Maximum distance to check for intersection.
/// @return true if Intersect
/////////////////////////////////////
bool SphereCollider::RaycastCollider(Ray const& ray, RaycastHit& hitInfo, float32 maxDistance)
{
	bool hasHit = false;
	hitInfo.distance = maxDistance;

	for (SphereCollider* sphere : SphereCollider::s_list)
	{
		if (!sphere->m_created) continue;

		Sphere const& s = sphere->m_worldSphere;

		Vector3f32 oc = ray.origin - s.center;
		float a = ray.direction.DotProduct(ray.direction);
		float b = 2.0f * oc.DotProduct(ray.direction);
		float c = oc.DotProduct(oc) - s.ray * s.ray;
		float discriminant = b * b - 4 * a * c;

		if (discriminant < 0) continue;

		float sqrtDisc = sqrt(discriminant);
		float t1 = (-b - sqrtDisc) / (2.0f * a);
		float t2 = (-b + sqrtDisc) / (2.0f * a);

		float t = (t1 >= 0 && t1 <= maxDistance) ? t1 : (t2 >= 0 && t2 <= maxDistance ? t2 : -1);
		if (t < 0) continue;

		hitInfo.point = ray.origin + ray.direction * t;
		hitInfo.normal = (hitInfo.point - s.center).Normalize();
		hitInfo.distance = t;
		hitInfo.pGameObject = &sphere->GetOwner();

		hasHit = true;
	}

	return hasHit;
}

#undef SPHERE_DEFAULT_VALUE

}