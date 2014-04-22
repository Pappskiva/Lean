#ifndef MATHSTRUCTS_H

#include <cstring>
#include <math.h>
#include "MathFuncs.h"

template <typename _Type>
struct		Vec2;
template <typename _Type>
struct		Vec3;
template <typename _Type>
struct		Matrix3;
template <typename _Type>
struct		Matrix4;
template <typename _Type>
struct		Quat4;

/*=============================================================================

	A 2-dimensional vector class with operators and methods for most usual
	vector operations.

=============================================================================*/

template <typename _Type>
struct Vec2
{
	union
	{
		_Type				v[2];
		struct
		{
			_Type			x;
			_Type			y;
		};
	};

							Vec2() {};
							Vec2(_Type x, _Type y);
							Vec2(_Type value);

	Vec2			operator	+	(const Vec2& in) const;
	Vec2			operator	-	(const Vec2& in) const;
	Vec2			operator	-	() const;
	_Type			operator	*	(const Vec2& in) const;
	Vec2			operator	*	(const _Type in) const;
	Vec2&			operator	=	(const Vec2& in);
	Vec2&			operator	=	(const _Type in);
	Vec2&			operator	+=	(const Vec2& in);
	Vec2&			operator	-=	(const Vec2& in);
	Vec2&			operator	*=	(const _Type in);
	bool			operator	==	(const Vec2& in) const;
	bool			operator	!=	(const Vec2& in) const;
	_Type&			operator	[]	(const int i);

	Vec2					GetNormalized() const;
	void					Normalize();
	_Type					Length() const;
	_Type					LengthSquare() const;
};


/*=============================================================================

	A 3-dimensional vector class with operators and methods for most usual
	vector operations.

=============================================================================*/

template <typename _Type>
struct Vec3
{
	union
	{
		_Type				v[3];
		struct
		{
			_Type			x;
			_Type			y;
			_Type			z;
		};
	};

							Vec3() {x = y = z = 0.0f;};
							Vec3(_Type x, _Type y, _Type z);
							Vec3(_Type value);

	Vec3			operator	+	(const Vec3 &in) const;
	Vec3			operator	-	(const Vec3 &in) const;
	Vec3			operator	-	() const;
	_Type			operator	*	(const Vec3 &in) const;
	Vec3			operator	*	(const _Type in) const;
	Vec3			operator	*	(const Matrix4<_Type> &in) const;
	Vec3			operator	*	(const Matrix3<_Type> &in) const;
	Vec3			operator	/	(const _Type in);
	Vec3&			operator	=	(const Vec3 &in);
	Vec3&			operator	=	(const _Type in);
	Vec3&			operator	+=	(const Vec3 &in);
	Vec3&			operator	-=	(const Vec3 &in);
	Vec3&			operator	*=	(const _Type in);
	Vec3&			operator	*=	(const Matrix4<_Type> &in);
	Vec3&			operator	*=	(const Matrix3<_Type> &in);
	bool			operator	==	(const Vec3 &in) const;
	bool			operator	!=	(const Vec3 &in) const;
	_Type&			operator	[]	(const int i);

	_Type					Dot(const Vec3 &in) const;
	Vec3					GetNormalized() const;
	void					Normalize();
	const bool				HasLength() const;
	_Type					Length() const;
	_Type					LengthSquare() const;
	Vec3					Cross(const Vec3 &p) const;
	void					PosFrom(const Matrix4<_Type> &m);

};

/*=============================================================================

	A 3 by 3 matrix class with operators and methods for most usual
	matrix operations.

=============================================================================*/

template <typename _Type>
struct Matrix3
{
	union
	{
		_Type				m[3][3];
		struct
		{
			_Type			_11, _12, _13;
			_Type			_21, _22, _23;
			_Type			_31, _32, _33;
		};
	};

							Matrix3();
							Matrix3(const Vec3<_Type> &col1, const Vec3<_Type> &col2, const Vec3<_Type> &col3);
							Matrix3(const _Type _11, const _Type _12, const _Type _13,
								const _Type _21, const _Type _22, const _Type _23,
								const _Type _31, const _Type _32, const _Type _33);
							Matrix3(const Matrix4<_Type>& in);
							Matrix3(const Matrix3& in);
							Matrix3(const Quat4<_Type> &in);
							Matrix3(const Vec3<_Type> &in);

	Matrix3					operator	*	(const Matrix3& in) const;
	Vec3<_Type>				operator	*	(const Vec3<_Type>& in) const;
	Matrix3 &				operator	=	(const Matrix3& in);
	Matrix3 &				operator	=	(const Matrix4<_Type>& in);
	Matrix3 &				operator	*=	(const Matrix3& in);
	void					operator	*=	(const _Type in);
	void					operator	+=	(const Matrix3& in);

	Matrix3 &				Identity();
	Vec3<_Type>				GetZDir() const;
	void					FromDir(const Vec3<_Type> &dir);
	void					FromDirUp(const Vec3<_Type> &dir, const Vec3<_Type> &up);
	void					OrientationFrom(const Matrix4<_Type> &m);
	void					OrientationFrom(const Quat4<_Type> &q);
	_Type					Determinant();
	void					Invert();
	void					SetColumns(const Vec3<_Type> &col1, const Vec3<_Type> &col2, const Vec3<_Type> &col3);
	void					SetSkewSymmetric(const Vec3<_Type> &v);
	Vec3<_Type>				TransformTranspose(const Vec3<_Type> &v) const;
	Vec3<_Type>				Transform(const Vec3<_Type> &v) const;
	Matrix3					GetTranspose() const;
	Matrix3&				Transpose();
	

	static Matrix3				CreateRotateX(const float a);		// pure X, Y, Z rotation matrices
	static Matrix3				CreateRotateY(const float a);		// (a in radians)
	static Matrix3				CreateRotateZ(const float a);

	const static Matrix3		IDENTITY;
};

/*=============================================================================

	A 4 by 4 matrix class with operators and methods for most usual
	matrix operations.

=============================================================================*/

template <typename _Type>
struct Matrix4
{
	union
	{
		_Type				m[4][4];
		struct
		{
			_Type			_11, _12, _13, _14;
			_Type			_21, _22, _23, _24;
			_Type			_31, _32, _33, _34;
			_Type			_41, _42, _43, _44;
		};
	};

							Matrix4() {};
							Matrix4(const Vec3<_Type> &pos, const Matrix3<_Type> &orientation);
							Matrix4(const _Type _11, const _Type _12, const _Type _13, const _Type _14,
								const _Type _21, const _Type _22, const _Type _23, const _Type _24,
								const _Type _31, const _Type _32, const _Type _33, const _Type _34,
								const _Type _41, const _Type _42, const _Type _43, const _Type _44);

	Matrix4					operator	*	(const Matrix4& in) const;
	Matrix4 &				operator	=	(const Matrix4& in);

	void					FromPosDir(const Vec3<_Type> &pos, const Matrix3<_Type> &dir);
	void					FromPosDir(const Vec3<_Type> &pos, const Quat4<_Type> &dir);
	void					FromPosDir(const Vec3<_Type> &pos, const Vec3<_Type> &dir);
	void					ViewAtLH(const Vec3<_Type> &pos, const Vec3<_Type> &at, const Vec3<_Type> &up);
	void					ViewAtLH(const Vec3<_Type> &pos, const Vec3<_Type> &at);
	void					ViewAlongLH(const Vec3<_Type> &pos, const Vec3<_Type> &dir);
	void					Projection(float fov, float zn, float zf, float aspectRatio);

	Matrix4						Inverse() const;
	const Matrix4&				Translate(const Vec3<_Type> &displacement);	
	const Matrix4&				LookAlong(const Vec3<_Type> &v);	
	_Type						Determinant() const;
	const Matrix4&				Transpose();
	Vec3<_Type>					TransformInverse(const Vec3<_Type> &vector) const;
	Vec3<_Type>					Transform(const Vec3<_Type> &vector) const;


	void						SetOrientation(const Matrix3<_Type> &in);
	void						SetOrientation(const Quat4<_Type> &q);
	void						SetOrientation(const Vec3<_Type> &v);
	void						SetPos(const Vec3<_Type>& pos);

	Vec3<_Type>					GetZDir() const;
	Vec3<_Type>					GetPos() const;
	Matrix3<_Type>				GetOrientation() const;
	
	static Matrix4				CreateIdentity();
	static Matrix4				CreateTranslation(const Vec3<_Type> &pos);
	static Matrix4				CreateRotateX(const float a);			// pure X, Y, Z rotation matrices
	static Matrix4				CreateRotateY(const float a);			// (a in radians)
	static Matrix4				CreateRotateZ(const float a);			//rename fo sho
	static Matrix4				CreateRotationFrom(const Quat4<_Type> &rot);
	static Matrix4				CreateOrthographicLH(const float width, const float height, const float zNear, const float zFar);
	static Matrix4				CreateYawPitchRoll(const float yaw, const float pitch, const float roll);
	static Matrix4				CreateScale(const Vec3<_Type> &in);

	const static Matrix4		IDENTITY;
};



/*=============================================================================

	A quaternion class with the needed quaternion operations.

=============================================================================*/

template <typename _Type>
struct Quat4
{
	union
	{
		_Type				q[4];
		struct
		{
			_Type			x;
			_Type			y;
			_Type			z;
			_Type			w;
		};
	};

	Quat4() {};
					Quat4(const _Type w, const _Type x, const _Type y, const _Type z);
	Quat4&			operator	=	(const Quat4& in);
	void			operator	*=	(const Quat4& in);
	Quat4			operator	*	(const Quat4& in);
	Vec3<_Type>		operator	*	(const Vec3<_Type> &in);
	const Quat4& 	AddScaledVector(const Vec3<_Type> &in, const _Type scale);
	void			RotateByVector(const Vec3<_Type> &in);
	void			Normalize();
	Quat4			GetNormalized();
	float			MagnitudeSquared();
	void			Conjugate();
	Quat4			GetConjugate();

	static Quat4	SetOrientation(const Vec3<_Type> &axis, const _Type angle);
};

#include "MathStructs.inl"

#define MATHSTRUCTS_H
#endif