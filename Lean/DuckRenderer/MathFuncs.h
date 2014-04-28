#ifndef MATH_H

#define			PI								3.14159265358979f
#define			EPSILON_TRACE					0.00001f
#define			EPSILON							0.00001f
#define			EPSILON_SQUARE					EPSILON * EPSILON

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

const float		FastSqrt(const float x);
const float		ToRadians(const float degrees);
const float		ToDegrees(const float radians);

template <typename _Type>
inline const _Type		Saturate(_Type value, const _Type minVal = (_Type) 0.0L, const _Type maxVal = (_Type) 1.0L)
{
	 return min(max(value, minVal), maxVal);
}

// definitions

inline const float ToRadians(const float degrees)
{
	return degrees * PI / 180.0f;
}

inline const float ToDegrees(const float radians)
{
	return radians * 180.0f / PI;
}

#define MATH_H
#endif