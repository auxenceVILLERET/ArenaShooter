#include "RenderTransform.h"

namespace gce
{

	RenderTransform::RenderTransform(Vector3f32 pos, Vector3f32 rot, Vector3f32 scale)
	{
		m_pos = pos;
		m_quaternionRot.SetRotationEuler(rot);
		m_scale = scale;
		UpdateRenderTransformMatrix();
	}

	RenderTransform::RenderTransform(RenderTransform const& other)
	{
		m_pos = other.m_pos;
		m_scale = other.m_scale;
		m_quaternionRot = other.m_quaternionRot;
		m_matrix = other.m_matrix;

		UpdateRenderTransformMatrix();
	}

	Vector3f32& RenderTransform::GetPosition()
	{
		return m_pos;
	}

	void RenderTransform::SetPosition(Vector3f32 const& pos)
	{
		m_pos = pos;
	}

	void RenderTransform::Rotate(float32 const& x, float32 const& y, float32 const& z)
	{
		m_quaternionRot *= Quaternion::RotationEuler(x,y,z);
	}

	void RenderTransform::UpdateRenderTransformMatrix()
	{
		m_matrix = 
			Matrix::Scale(m_scale) *
			Matrix::Rotation(m_quaternionRot) * 
			Matrix::Translation(m_pos);
	}


	Matrix RenderTransform::GetWorldMatrix() const
	{
		return m_matrix.GetTransposed();
	}

}
