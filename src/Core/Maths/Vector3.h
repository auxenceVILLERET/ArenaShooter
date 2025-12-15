#ifndef CORE_MATHS_VECTOR3_H_INCLUDED
#define CORE_MATHS_VECTOR3_H_INCLUDED

#include <xmmintrin.h>
#include "define.h"

namespace gce
{
    union Vector3f32
    {
    public:
        Vector3f32() = default;
        ~Vector3f32() = default;
        Vector3f32(float32 x, float32 y, float32 z);
        Vector3f32(float32 values[3]);
        Vector3f32(Vector3f32 const&);
        Vector3f32& operator=(Vector3f32 const&);

        void SetNull();
        void SetOne();

        [[nodiscard]] float32 Distance(Vector3f32 const& other) const;
        [[nodiscard]] float32 DotProduct(Vector3f32 const&) const;
        [[nodiscard]] float32 Norm() const;
        [[nodiscard]] float32 SquareNorm() const;

        [[nodiscard]] Vector3f32 CrossProduct(Vector3f32 const&) const;
        [[nodiscard]] Vector3f32 Normalize() const;
        Vector3f32& SelfNormalize();

        [[nodiscard]] Vector3f32 operator-(Vector3f32 const&) const;
        [[nodiscard]] Vector3f32 operator+(Vector3f32 const&) const;
        [[nodiscard]] Vector3f32 operator*(Vector3f32 const&) const;
        [[nodiscard]] Vector3f32 operator/(Vector3f32 const&) const;
        [[nodiscard]] Vector3f32 operator*(float32 scalar) const;
        [[nodiscard]] Vector3f32 operator/(float32 scalar) const;
        [[nodiscard]] Vector3f32 operator-() const;

        [[nodiscard]] float32& operator[](int8 index);
        [[nodiscard]] float32 const& operator[](int8 index) const;

        Vector3f32& operator+=(Vector3f32 const&);
        Vector3f32& operator-=(Vector3f32 const&);
        Vector3f32& operator*=(Vector3f32 const&);
        Vector3f32& operator/=(Vector3f32 const&);
        Vector3f32& operator*=(float32 scalar);
        Vector3f32& operator/=(float32 scalar);

        [[nodiscard]] bool operator==(Vector3f32 const&) const;
        [[nodiscard]] bool operator!=(Vector3f32 const&) const;

        [[nodiscard]] static Vector3f32 Lerp(Vector3f32 const& a, Vector3f32 const& b, float t);
        [[nodiscard]] static Vector3f32 Lerp(Vector3f32 const& a, Vector3f32 const& b, Vector3f32 const& t);

        struct 
        {
            float32 x;
            float32 y;
            float32 z;
        };

    private:
        float32 m_pData[3];
        Vector3f32(__m128 v);
        __m128 Load() const;
        void _vectorcall Store( __m128 v );
	};

    union Vector3i32
    {
    public:
        Vector3i32() = default;
        ~Vector3i32() = default;
        Vector3i32(int32 x, int32 y, int32 z);
        Vector3i32(int32 values[3]);
        Vector3i32(Vector3i32 const&);
        Vector3i32& operator=(Vector3i32 const&);

        void SetNull();
        void SetOne();
        
        [[nodiscard]] int32 DotProduct(Vector3i32 const&) const;
        [[nodiscard]] int32 Norm() const;
        [[nodiscard]] int32 SquareNorm() const;

        [[nodiscard]] Vector3i32 CrossProduct(Vector3i32 const&) const;
        [[nodiscard]] Vector3i32 Normalize() const;
        Vector3i32& SelfNormalize();

        [[nodiscard]] Vector3i32 operator-(Vector3i32 const&) const;
        [[nodiscard]] Vector3i32 operator+(Vector3i32 const&) const;
        [[nodiscard]] Vector3i32 operator*(Vector3i32 const&) const;
        [[nodiscard]] Vector3i32 operator/(Vector3i32 const&) const;
        [[nodiscard]] Vector3i32 operator*(int32 scalar) const;
        [[nodiscard]] Vector3i32 operator/(int32 scalar) const;
        [[nodiscard]] Vector3i32 operator-() const;

        [[nodiscard]] int32& operator[](int8 index);
        [[nodiscard]] int32 const& operator[](int8 index) const;

        Vector3i32& operator+=(Vector3i32 const&);
        Vector3i32& operator-=(Vector3i32 const&);
        Vector3i32& operator*=(Vector3i32 const&);
        Vector3i32& operator/=(Vector3i32 const&);
        Vector3i32& operator*=(int32 scalar);
        Vector3i32& operator/=(int32 scalar);

        [[nodiscard]] bool operator==(Vector3i32 const&) const;
        [[nodiscard]] bool operator!=(Vector3i32 const&) const;

        [[nodiscard]] static Vector3i32 Lerp(Vector3i32 const& a, Vector3i32 const& b, float t);
        [[nodiscard]] static Vector3i32 Lerp(Vector3i32 const& a, Vector3i32 const& b, Vector3i32 const& t);

        struct 
        {
            int32 x;
            int32 y;
            int32 z;
        };

    private:
        int32 m_pData[3];
        Vector3i32(__m128 v);
        __m128 Load() const;
        void _vectorcall Store( __m128 v );
	};
}
#endif
