#ifndef _DUCK_MATH_H
#define _DUCK_MATH_H

#include "MathStructs.h"

#define	DEF_MATH_PRECISION				float
#define	DEF_MATH_INTEGER				int

typedef	Vec2<DEF_MATH_PRECISION>		v2;
typedef	Vec3<DEF_MATH_PRECISION>		v3;
typedef	Matrix3<DEF_MATH_PRECISION>		m3;
typedef	Matrix4<DEF_MATH_PRECISION>		m4;
typedef Quat4<DEF_MATH_PRECISION>		q4;

typedef	Vec2<DEF_MATH_INTEGER>			v2i;
typedef	Vec3<DEF_MATH_INTEGER>			v3i;
typedef	Matrix3<DEF_MATH_INTEGER>		m3i;
typedef	Matrix4<DEF_MATH_INTEGER>		m4i;


#endif