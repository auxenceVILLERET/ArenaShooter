#pragma once
#include "Maths/Matrix.h"
#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"

namespace gce
{

	class RenderTransform
	{
	public:
		RenderTransform() = default;
		RenderTransform(Vector3f32 pos, Vector3f32 rot, Vector3f32 scale);

		RenderTransform(RenderTransform const& other);
		RenderTransform(RenderTransform&& other) = delete;

		RenderTransform& operator=(RenderTransform const& other) = delete;
		RenderTransform& operator=(RenderTransform&& other) = delete;

		~RenderTransform() = default;

		Vector3f32& GetPosition();
		Matrix GetWorldMatrix() const;

		void UpdateRenderTransformMatrix();
		void SetPosition(Vector3f32 const& pos);
		void Rotate(float32 const& x, float32 const& y, float32 const& z);

		Matrix m_matrix;
	private:

		Vector3f32 m_pos;
		Quaternion m_quaternionRot;
		Vector3f32 m_scale = { 1.0f, 1.0f, 1.0f };

		friend class RenderWindow;
		friend class RenderTarget;
		friend class RenderCamera;
	};


}
