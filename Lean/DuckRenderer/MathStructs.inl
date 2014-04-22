/*==================
Vec2<_Type>
==================*/

template <typename _Type>
Vec2<_Type>::Vec2(_Type x, _Type y)
{
	this->x = x;
	this->y = y;
}

template <typename _Type>
Vec2<_Type> Vec2<_Type>::operator + (const Vec2<_Type>& in) const
{
	return Vec2<_Type>(x +  in.x, y + in.y);
}

template <typename _Type>
Vec2<_Type> Vec2<_Type>::operator - (const Vec2<_Type>& in) const
{
	return Vec2<_Type>(x - in.x, y - in.y);
}

template <typename _Type>
Vec2<_Type> Vec2<_Type>::operator - () const
{
	return Vec2<_Type>(-x, -y);
}

template <typename _Type>
_Type Vec2<_Type>::operator * (const Vec2<_Type>& in) const
{
	return x * in.x + y * in.y;
}

template <typename _Type>
Vec2<_Type> Vec2<_Type>::operator * (const _Type in) const
{
	return Vec2<_Type>(x * in, y * in);
}

template <typename _Type>
Vec2<_Type>& Vec2<_Type>::operator = (const Vec2<_Type>& in)
{
	x = in.x;
	y = in.y;

	return *this;
}

template <typename _Type>
Vec2<_Type>& Vec2<_Type>::operator = (const _Type in)
{
	y = x = in;

	return *this;
}

template <typename _Type>
Vec2<_Type>& Vec2<_Type>::operator += (const Vec2<_Type>& in)
{
	x += in.x;
	y += in.y;

	return *this;
}

template <typename _Type>
Vec2<_Type>& Vec2<_Type>::operator -= (const Vec2<_Type>& in)
{
	x -= in.x;
	y -= in.y;

	return *this;
}

template <typename _Type>
Vec2<_Type>& Vec2<_Type>::operator *= (const _Type in)
{
	x *= in;
	y *= in;

	return *this;
}

template <typename _Type>
bool Vec2<_Type>::operator != (const Vec2<_Type> &in) const
{
	if(x == in.x && y == in.y)		// EPSILON!
		return false;

	return true;
}

template <typename _Type>
bool Vec2<_Type>::operator == (const Vec2<_Type> &in) const
{
	if(x != in.x || y != in.y)		// EPSILON!
		return false;

	return true;
}

template <typename _Type>
Vec2<_Type> Vec2<_Type>::GetNormalized() const
{
	_Type m = Length();
	if(m > 0.0f)
		return Vec2<_Type>(x / m, y / m);
	return Vec2<_Type>(0, 0);
}

template <typename _Type>
void Vec2<_Type>::Normalize()
{
	_Type m = Length();

	if(m)
	{
		x /= m;
		y /= m;
	}
}

template <typename _Type>
_Type Vec2<_Type>::Length() const
{
	_Type m =  sqrtf(x * x + y * y);
	
	if(m)
		return m;
	else
		return 0.0f;
}

template <typename _Type>
_Type Vec2<_Type>::LengthSquare() const
{
	return x * x + y * y;
}

/*==================
Vec3<_Type>
==================*/

template <typename _Type>
Vec3<_Type>::Vec3(_Type x, _Type y, _Type z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template <typename _Type>
Vec3<_Type>::Vec3(_Type in)
{
	z = y = x = in;
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::operator + (const Vec3<_Type>& in) const
{
	return Vec3<_Type>(x +  in.x, y + in.y, z + in.z);
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::operator - (const Vec3<_Type>& in) const
{
	return Vec3<_Type>(x - in.x, y - in.y, z - in.z);
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::operator - () const
{
	return Vec3<_Type>(-x, -y, -z);
}

template <typename _Type>
_Type Vec3<_Type>::operator * (const Vec3<_Type>& in) const
{
	return x * in.x + y * in.y + z * in.z;
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::operator * (const _Type in) const
{
	return Vec3<_Type>(x * in, y * in, z * in);
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::operator * (const Matrix3<_Type>& in) const
{
	Vec3<_Type> temp;

	temp.x = x * in.m[0][0] + y * in.m[0][1] + z * in.m[0][2];
	temp.y = x * in.m[1][0] + y * in.m[1][1] + z * in.m[1][2];
	temp.z = x * in.m[2][0] + y * in.m[2][1] + z * in.m[2][2];

	return temp;
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::operator * (const Matrix4<_Type>& in) const
{
	Vec3<_Type> temp;

	temp.x = x * in.m[0][0] + y * in.m[0][1] + z * in.m[0][2] + in.m[3][0];
	temp.y = x * in.m[1][0] + y * in.m[1][1] + z * in.m[1][2] + in.m[3][1];
	temp.z = x * in.m[2][0] + y * in.m[2][1] + z * in.m[2][2] + in.m[3][2];

	return temp;
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::operator / (const _Type in)
{
	return Vec3<_Type>(x / in, y / in, z / in);
}

template <typename _Type>
Vec3<_Type>& Vec3<_Type>::operator = (const Vec3<_Type>& in)
{
	x = in.x;
	y = in.y;
	z = in.z;

	return *this;
}

template <typename _Type>
Vec3<_Type>& Vec3<_Type>::operator = (const _Type in)
{
	z = y = x = in;

	return *this;
}

template <typename _Type>
Vec3<_Type>& Vec3<_Type>::operator += (const Vec3<_Type>& in)
{
	x += in.x;
	y += in.y;
	z += in.z;

	return *this;
}

template <typename _Type>
Vec3<_Type>& Vec3<_Type>::operator -= (const Vec3<_Type>& in)
{
	x -= in.x;
	y -= in.y;
	z -= in.z;

	return *this;
}

template <typename _Type>
Vec3<_Type>& Vec3<_Type>::operator *= (const _Type in)
{
	x *= in;
	y *= in;
	z *= in;

	return *this;
}

template <typename _Type>
Vec3<_Type>& Vec3<_Type>::operator *= (const Matrix3<_Type> &in)
{
	Vec3<_Type> temp = *this;

	v[0] = temp.x * in.m[0][0] + temp.y * in.m[1][0] + temp.z * in.m[2][0];
	v[1] = temp.x * in.m[0][1] + temp.y * in.m[1][1] + temp.z * in.m[2][1];
	v[2] = temp.x * in.m[0][2] + temp.y * in.m[1][2] + temp.z * in.m[2][2];

	return *this;
}

template <typename _Type>
bool Vec3<_Type>::operator != (const Vec3<_Type> &in) const
{
	if(x == in.x && y == in.y && z == in.z)		// EPSILON!
		return false;

	return true;
}

template <typename _Type>
bool Vec3<_Type>::operator == (const Vec3<_Type> &in) const
{
	if(x != in.x || y != in.y || z != in.z)		// EPSILON!
		return false;

	return true;
}

template <typename _Type>
_Type Vec3<_Type>::Dot(const Vec3<_Type> &in) const
{
	return *this * in;
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::Cross(const Vec3<_Type> &p) const
{
	Vec3<_Type> out;
	out.x = y * p.z - z * p.y;
	out.y = z * p.x - x * p.z;
	out.z = x * p.y - y * p.x;

	return out;
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::GetNormalized() const
{
	_Type m = Length();
	if(m > 0.0f)
		return Vec3<_Type>(x / m, y / m, z / m);
	return Vec3<_Type>(0,0,0);
}

template <typename _Type>
void Vec3<_Type>::Normalize()
{
	_Type m = Length();

	if(m)
	{
		x /= m;
		y /= m;
		z /= m;
	}
}

template <typename _Type>
const bool Vec3<_Type>::HasLength() const
{
	return x != 0.0 || y != 0.0f || z != 0.0f;
}

template <typename _Type>
_Type Vec3<_Type>::Length() const
{
	_Type m =  sqrtf(x * x + y * y + z * z);
	
	if(m)
		return m;
	else
		return 0.0f;
}

template <typename _Type>
_Type Vec3<_Type>::LengthSquare() const
{
	return x * x + y * y + z * z;
}

template <typename _Type>
void Vec3<_Type>::PosFrom(const Matrix4<_Type> &m)
{
	x = m._41;
	y = m._42;
	z = m._43;
}

template <typename _Type>
Vec3<_Type> Vec3<_Type>::Reflect(const Vec3 &normal)
{
	Vec3 dir = *this;
	return dir - normal * ((dir * normal * 2) / normal.LengthSquare());
}

/*==================
Matrix3<_Type>
==================*/

template <typename _Type>
Matrix3<_Type>::Matrix3()
{
}

template <typename _Type>
Matrix3<_Type>::Matrix3(const Vec3<_Type> &col1, const Vec3<_Type> &col2, const Vec3<_Type> &col3)
{
	_11 = col1.x;	_12 = col2.x;	_13 = col3.x;
	_21 = col1.y;	_22 = col2.y;	_23 = col3.y;
	_31 = col1.z;	_32 = col2.z;	_33 = col3.z;
}

template <typename _Type>
Matrix3<_Type>::Matrix3(	const _Type _11, const _Type _12, const _Type _13,
		const _Type _21, const _Type _22, const _Type _23,
		const _Type _31, const _Type _32, const _Type _33)
{
	this->_11 = _11;	this->_12 = _12;	this->_13 = _13;
	this->_21 = _21;	this->_22 = _22;	this->_23 = _23;
	this->_31 = _31;	this->_32 = _32;	this->_33 = _33;
}

template <typename _Type>
Matrix3<_Type>::Matrix3(const Matrix4<_Type>& in)
{
	_11 = in._11;	_12 = in._12;	_13 = in._13;
	_21 = in._21;	_22 = in._22;	_23 = in._23;
	_31 = in._31;	_32 = in._32;	_33 = in._33;
}

template <typename _Type>
Matrix3<_Type>::Matrix3(const Matrix3<_Type>& in)
{
	_11 = in._11;	_12 = in._12;	_13 = in._13;
	_21 = in._21;	_22 = in._22;	_23 = in._23;
	_31 = in._31;	_32 = in._32;	_33 = in._33;
}

template <typename _Type>
Matrix3<_Type>::Matrix3(const Quat4<_Type> &q)
{
	_11 = 1 - 2 * (q.y * q.y + q.z * q.z);
	_12 = 2 * (q.x * q.y - q.z * q.w);
	_13 = 2 * (q.x * q.z + q.y * q.w);

	_21 = 2 * (q.x * q.y + q.z * q.w);
	_22 = 1 - 2 * (q.x * q.x + q.z * q.z);
	_23 = 2 * (q.y * q.z - q.x * q.w);

	_31 = 2 * (q.x * q.z - q.y * q.w);	
	_32 = 2 * (q.y * q.z + q.x * q.w);	
	_33 = 1 - 2 * (q.x * q.x + q.y * q.y);
}

template <typename _Type>
Matrix3<_Type>::Matrix3(const Vec3<_Type> &v)
{
	Vec3<_Type> up = Vec3<_Type>(0.0f, 1.0f, 0.0f);
	Vec3<_Type> z = v.GetNormalized();
	Vec3<_Type> x = up.Cross(z);
	Vec3<_Type> y = z.Cross(x).GetNormalized();

	_11 = x.x;		_12 = x.y;		_13 = x.z;
	_21 = y.x;		_22 = y.y;		_23 = y.z;
	_31 = z.x;		_32 = z.y;		_33 = z.z;
}

template <typename _Type>
Matrix3<_Type> Matrix3<_Type>::operator	*(const Matrix3<_Type>& in) const
{
	Matrix3<_Type> out;

	out._11 = _11 * in._11 + _12 * in._21 + _13 * in._31;
	out._21 = _21 * in._11 + _22 * in._21 + _23 * in._31;
	out._31 = _31 * in._11 + _32 * in._21 + _33 * in._31;

	out._12 = _11 * in._12 + _12 * in._22 + _13 * in._32;
	out._22 = _21 * in._12 + _22 * in._22 + _23 * in._32;
	out._32 = _31 * in._12 + _32 * in._22 + _33 * in._32;

	out._13 = _11 * in._13 + _12 * in._23 + _13 * in._33;
	out._23 = _21 * in._13 + _22 * in._23 + _23 * in._33;
	out._33 = _31 * in._13 + _32 * in._23 + _33 * in._33;

	return out;
}

template <typename _Type>
Vec3<_Type> Matrix3<_Type>::operator *(const Vec3<_Type> &in) const
{
	Vec3<_Type> out;

	out.x = _11 * in.x + _12 * in.y + _13 * in.z;
	out.y = _21 * in.x + _22 * in.y + _23 * in.z;
	out.z = _31 * in.x + _32 * in.y + _33 * in.z;

	return out;
}

template <typename _Type>
Matrix3<_Type>& Matrix3<_Type>::operator =(const Matrix3<_Type>& in)
{
	_11 = in._11;	_12 = in._12;	_13 = in._13;
	_21 = in._21;	_22 = in._22;	_23 = in._23;
	_31 = in._31;	_32 = in._32;	_33 = in._33;

	return *this;
}

template <typename _Type>
Matrix3<_Type>& Matrix3<_Type>::operator =(const Matrix4<_Type>& in)
{
	_11 = in._11;	_12 = in._12;	_13 = in._13;
	_21 = in._21;	_22 = in._22;	_23 = in._23;
	_31 = in._31;	_32 = in._32;	_33 = in._33;

	return *this;
}

template <typename _Type>
Matrix3<_Type>& Matrix3<_Type>::operator *=(const Matrix3<_Type>& in)
{
	Matrix3<_Type> temp = *this;

	temp._11 = _11 * in._11 + _12 * in._21 + _13 * in._31;
	temp._21 = _21 * in._11 + _22 * in._21 + _23 * in._31;
	temp._31 = _31 * in._11 + _32 * in._21 + _33 * in._31;

	temp._12 = _11 * in._12 + _12 * in._22 + _13 * in._32;
	temp._22 = _21 * in._12 + _22 * in._22 + _23 * in._32;
	temp._32 = _31 * in._12 + _32 * in._22 + _33 * in._32;

	temp._13 = _11 * in._13 + _12 * in._23 + _13 * in._33;
	temp._23 = _21 * in._13 + _22 * in._23 + _23 * in._33;
	temp._33 = _31 * in._13 + _32 * in._23 + _33 * in._33;

	return *this = temp;
}

template <typename _Type>
void Matrix3<_Type>::operator *=(const _Type in)
{
	_11 *= in;	_12 *= in;	_13 *= in;
	_21 *= in;	_22 *= in;	_23 *= in;
	_31 *= in;	_32 *= in;	_33 *= in;
}

template <typename _Type>
void Matrix3<_Type>::operator +=(const Matrix3<_Type> &in)
{
	_11 += in._11;	_12 += in._12;	_13 += in._13;
	_21 += in._21;	_22 += in._22;	_23 += in._23;
	_31 += in._31;	_32 += in._32;	_33 += in._33;
}

template <typename _Type>
Matrix3<_Type> & Matrix3<_Type>::Identity()
{
	_11 = 1.0f;		_12 = 0.0f;		_13 = 0.0f;
	_21 = 0.0f;		_22 = 1.0f;		_23 = 0.0f;
	_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;

	return *this;
}

template <typename _Type>
Vec3<_Type> Matrix3<_Type>::GetZDir() const
{
	return Vec3<_Type>(_31, _32, _33);
}

template <typename _Type>
void Matrix3<_Type>::FromDir(const Vec3<_Type> &dir)
{
	Vec3<_Type> up = Vec3<_Type>(0.0f, 1.0f, 0.0f);
	Vec3<_Type> z = dir.GetNormalized();
	Vec3<_Type> x = up.Cross(z);
	Vec3<_Type> y = z.Cross(x).GetNormalized();

	_11 = x.x;		_12 = x.y;		_13 = x.z;
	_21 = y.x;		_22 = y.y;		_23 = y.z;
	_31 = z.x;		_32 = z.y;		_33 = z.z;
}

template <typename _Type>
void Matrix3<_Type>::OrientationFrom(const Matrix4<_Type> &m)
{
	_11 = m._11;	_12 = m._12;	_13 = m._13;
	_21 = m._21;	_22 = m._22;	_23 = m._23;
	_31 = m._31;	_32 = m._32;	_33 = m._33;
}

template <typename _Type>
void Matrix3<_Type>::OrientationFrom(const Quat4<_Type> &q)
{
	_11 = 1 - 2 * (q.y * q.y + q.z * q.z);
	_12 = 2 * (q.x * q.y - q.z * q.w);
	_13 = 2 * (q.x * q.z + q.y * q.w);

	_21 = 2 * (q.x * q.y + q.z * q.w);
	_22 = 1 - 2 * (q.x * q.x + q.z * q.z);
	_23 = 2 * (q.y * q.z - q.x * q.w);

	_31 = 2 * (q.x * q.z - q.y * q.w);	
	_32 = 2 * (q.y * q.z + q.x * q.w);	
	_33 = 1 - 2 * (q.x * q.x + q.y * q.y);
}

template <typename _Type>
_Type Matrix3<_Type>::Determinant()
{
	return _11 * (_22 * _33 - _23 * _32) - _21 * (_12 * _33 - _13 * _32) + _31 * (_12 * _23 - _13 * _22);
}

template <typename _Type>
void Matrix3<_Type>::Invert()
{
	float det = Determinant();

	if(det == 0.0f)
		return;

	Matrix3<_Type> m = *this;
	float invD = 1.0f / det;

	_11 = (m._22 * m._33 - m._23 * m._32) * invD;
	_12 = -(m._12 * m._33 - m._13 * m._32) * invD;
	_13 = (m._12 * m._23 - m._13 * m._22) * invD;
	
	_21 = -(m._21 * m._33 - m._23 * m._31) * invD;
	_22 = (m._11 * m._33 - m._13 * m._31) * invD;
	_23 = -(m._11 * m._23 - m._13 * m._21) * invD;

	_31 = (m._21 * m._32 - m._22 * m._31) * invD;
	_32 = -(m._11 * m._32 - m._12 * m._31) * invD;
	_33 = (m._11 * m._22 - m._12 * m._21) * invD;
}

template <typename _Type>
void Matrix3<_Type>::SetColumns(const Vec3<_Type> &col1, const Vec3<_Type> &col2, const Vec3<_Type> &col3)
{
	_11 = col1.x;	_12 = col2.x;	_13 = col3.x;
	_21 = col1.y;	_22 = col2.y;	_23 = col3.y;
	_31 = col1.z;	_32 = col2.z;	_33 = col3.z;
}

template <typename _Type>
void Matrix3<_Type>::SetSkewSymmetric(const Vec3<_Type> &v)
{
	_11 = _22 = _33 = 0.0f;
	_12 = -v.z;
	_13 = v.y;
	_21 = v.z;
	_23 = -v.x;
	_31 = -v.y;
	_32 = v.x;
}

template <typename _Type>
Vec3<_Type> Matrix3<_Type>::TransformTranspose(const Vec3<_Type> &v) const
{
	return Vec3<_Type>(
		v.x * _11 + v.y * _21 + v.z * _31,
		v.x * _12 + v.y * _22 + v.z * _32,
		v.x * _13 + v.y * _23 + v.z * _33
		);
}

template <typename _Type>
Vec3<_Type> Matrix3<_Type>::Transform(const Vec3<_Type> &v) const
{
	return v * *this;
}

template <typename _Type>
Matrix3<_Type> Matrix3<_Type>::GetTranspose() const
{
	Matrix3<_Type> out;

	out.m[0][0] = m[0][0];
	out.m[1][0] = m[0][1];
	out.m[2][0] = m[0][2];

	out.m[0][1] = m[1][0];
	out.m[1][1] = m[1][1];
	out.m[2][1] = m[1][2];

	out.m[0][2] = m[2][0];
	out.m[1][2] = m[2][1];
	out.m[2][2] = m[2][2];

	return out;
}

template <typename _Type>
Matrix3<_Type>& Matrix3<_Type>::Transpose()
{
	Matrix3<_Type> temp = *this;

	m[0][0] = temp.m[0][0];
	m[1][0] = temp.m[0][1];
	m[2][0] = temp.m[0][2];

	m[0][1] = temp.m[1][0];
	m[1][1] = temp.m[1][1];
	m[2][1] = temp.m[1][2];

	m[0][2] = temp.m[2][0];
	m[1][2] = temp.m[2][1];
	m[2][2] = temp.m[2][2];

	return *this;
}

template <typename _Type>
Matrix3<_Type> Matrix3<_Type>::CreateRotateX(const float a)
{
	Matrix3<_Type> out;
	out._11 = 1.0f;		out._12 = 0.0f;		out._13 = 0.0f;
	out._21 = 0.0f;		out._22 = cos(a);	out._23 = -sin(a);
	out._31 = 0.0f;		out._32 = sin(a);	out._33 = cos(a);

	return out;
}

template <typename _Type>
Matrix3<_Type> Matrix3<_Type>::CreateRotateY(const float a)
{
	Matrix3<_Type> out;
	out._11 = cos(a);	out._12 = 0.0f;		out._13 = sin(a);
	out._21 = 0.0f;		out._22 = 1.0f;		out._23 = 0.0f;
	out._31 = -sin(a);	out._32 = 0.0f;		out._33 = cos(a);

	return out;
}

template <typename _Type>
Matrix3<_Type> Matrix3<_Type>::CreateRotateZ(const float a)
{
	Matrix3<_Type> out;
	out._11 = cos(a);	out._12 = -sin(a);	out._13 = 0.0f;
	out._21 = sin(a);	out._22 = cos(a);	out._23 = 0.0f;
	out._31 = 0.0f;		out._32 = 0.0f;		out._33 = 1.0f;

	return out;
}

template <typename _Type>
const Matrix3<_Type> Matrix3<_Type>::IDENTITY(1.0f, 0.0f, 0.0f, 
											  0.0f, 1.0f, 0.0f, 
											  0.0f, 0.0f, 1.0f);

/*==================
Matrix4<_Type>
==================*/

template <typename _Type>
Matrix4<_Type>::Matrix4(const _Type _11, const _Type _12, const _Type _13, const _Type _14,
								const _Type _21, const _Type _22, const _Type _23, const _Type _24,
								const _Type _31, const _Type _32, const _Type _33, const _Type _34,
								const _Type _41, const _Type _42, const _Type _43, const _Type _44)
{
	this->_11 = _11;	this->_12 = _12;	this->_13 = _13;	this->_14 = _14;
	this->_21 = _21;	this->_22 = _22;	this->_23 = _23;	this->_24 = _24;
	this->_31 = _31;	this->_32 = _32;	this->_33 = _33;	this->_34 = _34;
	this->_41 = _31;	this->_42 = _32;	this->_43 = _33;	this->_44 = _44;
}

template <typename _Type>
Matrix4<_Type>::Matrix4(const Vec3<_Type> &pos, const Matrix3<_Type> &orientation)
{
	FromPosDir(pos, orientation);
}

template <typename _Type>
Matrix4<_Type> & Matrix4<_Type>::operator = (const Matrix4<_Type>& in)
{
	_11 = in._11;	_12 = in._12;	_13 = in._13;	_14 = in._14;
	_21 = in._21;	_22 = in._22;	_23 = in._23;	_24 = in._24;
	_31 = in._31;	_32 = in._32;	_33 = in._33;	_34 = in._34;
	_41 = in._41;	_42 = in._42;	_43 = in._43;	_44 = in._44;

	return *this;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::operator * (const Matrix4<_Type>& in) const
{
	Matrix4<_Type> out;

	out._11 = _11 * in._11	+	_12 * in._21	+	_13 * in._31	+	_14 * in._41;
	out._21 = _21 * in._11	+	_22 * in._21	+	_23 * in._31	+	_24 * in._41;
	out._31 = _31 * in._11	+	_32 * in._21	+	_33 * in._31	+	_34 * in._41;
	out._41 = _41 * in._11	+	_42 * in._21	+	_43 * in._31	+	_44 * in._41;

	out._12 = _11 * in._12	+	_12 * in._22	+	_13 * in._32	+	_14 * in._42;
	out._22 = _21 * in._12	+	_22 * in._22	+	_23 * in._32	+	_24 * in._42;
	out._32 = _31 * in._12	+	_32 * in._22	+	_33 * in._32	+	_34 * in._42;
	out._42 = _41 * in._12	+	_42 * in._22	+	_43 * in._32	+	_44 * in._42;

	out._13 = _11 * in._13	+	_12 * in._23	+	_13 * in._33	+	_14 * in._43;
	out._23 = _21 * in._13	+	_22 * in._23	+	_23 * in._33	+	_24 * in._43;
	out._33 = _31 * in._13	+	_32 * in._23	+	_33 * in._33	+	_34 * in._43;
	out._43 = _41 * in._13	+	_42 * in._23	+	_43 * in._33	+	_44 * in._43;
		
	out._14 = _11 * in._14	+	_12 * in._24	+	_13 * in._34	+	_14 * in._44;
	out._24 = _21 * in._14	+	_22 * in._24	+	_23 * in._34	+	_24 * in._44;
	out._34 = _31 * in._14	+	_32 * in._24	+	_33 * in._34	+	_34 * in._44;
	out._44 = _41 * in._14	+	_42 * in._24	+	_43 * in._34	+	_44 * in._44;

	return out;
}

template <typename _Type>
void Matrix4<_Type>::FromPosDir(const Vec3<_Type> &pos, const Matrix3<_Type> &dir)
{
	_11 = dir._11;	_12 = dir._12;	_13 = dir._13;	_14 = 0.0f;
	_21 = dir._21;	_22 = dir._22;	_23 = dir._23;	_24 = 0.0f;
	_31 = dir._31;	_32 = dir._32;	_33 = dir._33;	_34 = 0.0f;
	_41 = pos.x;	_42 = pos.y;	_43 = pos.z;	_44 = 1.0f;
}

template <typename _Type>
void Matrix4<_Type>::FromPosDir(const Vec3<_Type> &pos, const Quat4<_Type> &q)
{
	_11 = 1 - 2 * (q.y * q.y + q.z * q.z);
	_12 = 2 * (q.x * q.y - q.z * q.w);
	_13 = 2 * (q.x * q.z + q.y * q.w);

	_21 = 2 * (q.x * q.y + q.z * q.w);
	_22 = 1 - 2 * (q.x * q.x + q.z * q.z);
	_23 = 2 * (q.y * q.z - q.x * q.w);

	_31 = 2 * (q.x * q.z - q.y * q.w);	
	_32 = 2 * (q.y * q.z + q.x * q.w);	
	_33 = 1 - 2 * (q.x * q.x + q.y * q.y);
	

	_41 = pos.x;
	_42 = pos.y;
	_43 = pos.z;
	
	_14 = 0.0f;
	_24 = 0.0f;
	_34 = 0.0f;
	_44 = 1.0f;
}

template <typename _Type>
void Matrix4<_Type>::FromPosDir(const Vec3<_Type> &pos, const Vec3<_Type> &v)
{
	Vec3<_Type> up = Vec3<_Type>(0.0f, 1.0f, 0.0f);
	Vec3<_Type> z = v.GetNormalized();
	Vec3<_Type> x = up.Cross(z);
	Vec3<_Type> y = z.Cross(x).GetNormalized();

	_11 = x.x;		_12 = x.y;		_13 = x.z;
	_21 = y.x;		_22 = y.y;		_23 = y.z;
	_31 = z.x;		_32 = z.y;		_33 = z.z;

	_41 = pos.x;
	_42 = pos.y;
	_43 = pos.z;
	
	_14 = 0.0f;
	_24 = 0.0f;
	_34 = 0.0f;
	_44 = 1.0f;
}

template <typename _Type>
void Matrix4<_Type>::ViewAtLH(const Vec3<_Type> &pos, const Vec3<_Type> &at, const Vec3<_Type> &up)
{
	Vec3<_Type> z = (at - pos);	z.Normalize();
	Vec3<_Type> x = up.Cross(z); x.Normalize();
	Vec3<_Type> y = z.Cross(x);
	float pX = -(x * pos);
	float pY = -(y * pos);
	float pZ = -(z * pos);

	_11 = x.x;		_12 = y.x;		_13 = z.x;		_14 = 0.0f;
	_21 = x.y;		_22 = y.y;		_23 = z.y;		_24 = 0.0f;
	_31 = x.z;		_32 = y.z;		_33 = z.z;		_34 = 0.0f;
	_41 = pX;		_42 = pY;		_43 = pZ;		_44 = 1.0f;
}

template <typename _Type>
void Matrix4<_Type>::ViewAtLH(const Vec3<_Type> &pos, const Vec3<_Type> &at)
{
	Vec3<_Type> z = (at - pos);	z.Normalize();
	Vec3<_Type> x = Vec3<_Type>(0.0f, 1.0f, 0.0f).Cross(z); x.Normalize();
	Vec3<_Type> y = z.Cross(x);

	float pX = -(x * pos);
	float pY = -(y * pos);
	float pZ = -(z * pos);

	_11 = x.x;		_12 = y.x;		_13 = z.x;		_14 = 0.0f;
	_21 = x.y;		_22 = y.y;		_23 = z.y;		_24 = 0.0f;
	_31 = x.z;		_32 = y.z;		_33 = z.z;		_34 = 0.0f;
	_41 = pX;		_42 = pY;		_43 = pZ;		_44 = 1.0f;
}

template <typename _Type>
void Matrix4<_Type>::ViewAlongLH(const Vec3<_Type> &pos, const Vec3<_Type> &dir)
{
	Vec3<_Type> z = dir.GetNormalized();
	Vec3<_Type> x = Vec3<_Type>(0.0f, 1.0f, 0.0f).Cross(z).GetNormalized();
	Vec3<_Type> y = z.Cross(x);
	float pX = -(x * pos);
	float pY = -(y * pos);
	float pZ = -(z * pos);

	_11 = x.x;		_12 = y.x;		_13 = z.x;		_14 = 0.0f;
	_21 = x.y;		_22 = y.y;		_23 = z.y;		_24 = 0.0f;
	_31 = x.z;		_32 = y.z;		_33 = z.z;		_34 = 0.0f;
	_41 = pX;		_42 = pY;		_43 = pZ;		_44 = 1.0f;
}

template <typename _Type>
void Matrix4<_Type>::Projection(float fov, float zn, float zf, float aspectRatio)
{
	float yScale = 1.0f / tan(fov / 2.0f);
	float xScale = yScale / aspectRatio;
	float x = zf / (zf - zn);

	_11 = xScale;	_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
	_21 = 0.0f;		_22 = yScale;	_23 = 0.0f;		_24 = 0.0f;
	_31 = 0.0f;		_32 = 0.0f;		_33 = x;		_34 = 1.0f;
	_41 = 0.0f;		_42 = 0.0f;		_43 = -zn * x;	_44 = 0.0f;
}

template <typename _Type>
void Matrix4<_Type>::SetOrientation(const Matrix3<_Type> &in)
{
	_11 = in._11;	_12 = in._12;	_13 = in._13;
	_21 = in._21;	_22 = in._22;	_23 = in._23;
	_31 = in._31;	_32 = in._32;	_33 = in._33;
}

template <typename _Type>
void Matrix4<_Type>::SetOrientation(const Quat4<_Type> &q)
{
	_11 = 1 - 2 * (q.y * q.y + q.z * q.z);
	_12 = 2 * (q.x * q.y - q.z * q.w);
	_13 = 2 * (q.x * q.z + q.y * q.w);

	_21 = 2 * (q.x * q.y + q.z * q.w);
	_22 = 1 - 2 * (q.x * q.x + q.z * q.z);
	_23 = 2 * (q.y * q.z - q.x * q.w);

	_31 = 2 * (q.x * q.z - q.y * q.w);	
	_32 = 2 * (q.y * q.z + q.x * q.w);	
	_33 = 1 - 2 * (q.x * q.x + q.y * q.y);
}

template <typename _Type>
void Matrix4<_Type>::SetOrientation(const Vec3<_Type> &v)
{
	Vec3<_Type> up = Vec3<_Type>(0.0f, 1.0f, 0.0f);
	Vec3<_Type> z = v.GetNormalized();
	Vec3<_Type> x = up.Cross(z);
	Vec3<_Type> y = z.Cross(x).GetNormalized();

	_11 = x.x;		_12 = x.y;		_13 = x.z;
	_21 = y.x;		_22 = y.y;		_23 = y.z;
	_31 = z.x;		_32 = z.y;		_33 = z.z;
}

template <typename _Type>
void Matrix4<_Type>::SetPos(const Vec3<_Type> &pos)
{
	_41 = pos.x;
	_42 = pos.y;
	_43 = pos.z;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::Inverse() const
{
	float det = Determinant();

	if(det == 0)
		return IDENTITY;

	det = 1 / det;

	Matrix4<_Type> out;

	out._11 = _22 * _33 * _44	+	_23 * _34 * _42		+	_24 * _32 * _43
		-	  _22 * _34 * _43	-	_23 * _32 * _44		-	_24 * _33 * _42;

	out._12 = _12 * _34 * _43	+	_13 * _32 * _44		+	_14 * _33 * _42
		-	  _12 * _33 * _44	-	_13 * _34 * _42		-	_14 * _32 * _43;

	out._13 = _12 * _23 * _44	+	_13 * _24 * _42		+	_14 * _22 * _43
		-	  _12 * _24 * _43	-	_13 * _22 * _44		-	_14 * _23 * _42;

	out._14 = _12 * _24 * _33	+	_13 * _22 * _34		+	_14 * _23 * _32
		-	  _12 * _23 * _34	-	_13 * _24 * _32		-	_14 * _22 * _33;


	out._21 = _21 * _34 * _43	+	_23 * _31 * _44		+	_24 * _33 * _41
		-	  _21 * _33 * _44	-	_23 * _34 * _41		-	_24 * _31 * _43;

	out._22 = _11 * _33 * _44	+	_13 * _34 * _41		+	_14 * _31 * _43
		-	  _11 * _34 * _43   -	_13 * _31 * _44		-	_14 * _33 * _41;

	out._23 = _11 * _24 * _43	+	_13 * _21 * _44		+	_14 * _23 * _41
		-	  _11 * _23 * _44	-	_13 * _24 * _41		-	_14 * _21 * _43;

	out._24 = _11 * _23 * _34	+	_13 * _24 * _31		+	_14 * _21 * _33
		-     _11 * _24 * _33	-	_13 * _21 * _34		-	_14 * _23 * _31;


	out._31 = _21 * _32 * _44	+	_22 * _34 * _41		+	_24 * _31 * _42
		-	  _21 * _34 * _42	-	_22 * _31 * _44		-	_24 * _32 * _41;

	out._32 = _11 * _34 * _42	+	_12 * _31 * _44		+	_14 * _32 * _41
		-	  _11 * _32 * _44	-	_12 * _34 * _41		-	_14 * _31 * _42;

	out._33 = _11 * _22 * _44	+	_12 * _24 * _41		+	_14 * _21 * _42 
		-	  _11 * _24 * _42	-	_12 * _21 * _44		-	_14 * _22 * _41;

	out._34	= _11 * _24 * _32	+	_12 * _21 * _34		+	_14 * _22 * _31 
		-	  _11 * _22 * _34	-	_12 * _24 * _31		-	_14 * _21 * _32;


	out._41 = _21 * _33 * _42	+	_22 * _31 * _43		+	_23 * _32 * _41
		-	  _21 * _32 * _43	-	_22 * _33 * _41		-	_23 * _31 * _42;

	out._42 = _11 * _32 * _43	+	_12 * _33 * _41		+	_13 * _31 * _42
		-	  _11 * _33 * _42	-	_12 * _31 * _43		-	_13 * _32 * _41;

	out._43 = _11 * _23 * _42	+	_12 * _21 * _43		+	_13 * _22 * _41
		-	  _11 * _22 * _43	-	_12 * _23 * _41		-	_13 * _21 * _42;

	out._44 = _11 * _22 * _33	+	_12 * _23 * _31		+	_13 * _21 * _32
		-	  _11 * _23 * _32	-	_12 * _21 * _33		-	_13 * _22 * _31;

	/*out._11 = _22 * _33 * _44	+	_23 * _34 * _42		+	_24 * _32 * _43
		-	 (_22 * _34 * _43	+	_23 * _32 * _44		+	_24 * _33 * _42);

	out._12 = _12 * _34 * _43	+	_13 * _32 * _44		+	_14 * _33 * _42
		-	 (_12 * _33 * _44	+	_13 * _34 * _42		+	_14 * _32 * _43);

	out._13 = _12 * _23 * _44	+	_13 * _24 * _42		+	_14 * _22 * _43
		-	 (_12 * _24 * _43	+	_13 * _22 * _44		+	_14 * _23 * _42);

	out._14 = _12 * _24 * _33	+	_13 * _22 * _34		+	_14 * _23 * _32
		-	 (_12 * _23 * _34	+	_13 * _24 * _32		+	_14 * _22 * _33);


	out._21 = _21 * _34 * _43	+	_23 * _31 * _44		+	_24 * _33 * _41
		-	 (_21 * _33 * _44	+	_23 * _34 * _41		+	_24 * _31 * _43);

	out._22 = _11 * _33 * _44	+	_13 * _34 * _41		+	_14 * _31 * _43
		-	 (_11 * _34 * _43   +	_13 * _31 * _44		+	_14 * _33 * _41);

	out._23 = _11 * _24 * _43	+	_13 * _21 * _44		+	_14 * _23 * _41
		-	 (_11 * _23 * _44	+	_13 * _24 * _41		+	_14 * _21 * _43);

	out._24 = _11 * _23 * _34	+	_13 * _24 * _31		+	_14 * _21 * _33
		-    (_11 * _24 * _33	+	_13 * _21 * _34		+	_14 * _23 * _31);


	out._31 = _21 * _32 * _44	+	_22 * _34 * _41		+	_24 * _31 * _42
		-	 (_21 * _34 * _42	+	_22 * _31 * _44		+	_24 * _32 * _41);

	out._32 = _11 * _34 * _42	+	_12 * _31 * _44		+	_14 * _32 * _41
		-	 (_11 * _32 * _44	+	_12 * _34 * _41		+	_14 * _31 * _42);

	out._33 = _11 * _22 * _44	+	_12 * _24 * _41		+	_14 * _21 * _42 
		-	 (_11 * _24 * _42	+	_12 * _21 * _44		+	_14 * _22 * _41);

	out._34	= _11 * _24 * _32	+	_12 * _21 * _34		+	_14 * _22 * _31 
		-	 (_11 * _22 * _34	+	_12 * _24 * _31		+	_14 * _21 * _32);


	out._41 = _21 * _33 * _42	+	_22 * _31 * _43		+	_23 * _32 * _41
		-	 (_21 * _32 * _43	-	_22 * _33 * _41		+	_23 * _31 * _42);

	out._42 = _11 * _32 * _43	+	_12 * _33 * _41		+	_13 * _31 * _42
		-	 (_11 * _33 * _42	+	_12 * _31 * _43		+	_13 * _32 * _41);

	out._43 = _11 * _23 * _42	+	_12 * _21 * _43		+	_13 * _22 * _41
		-	 (_11 * _22 * _43	+	_12 * _23 * _41		+	_13 * _21 * _42);

	out._44 = _11 * _22 * _33	+	_12 * _23 * _31		+	_13 * _21 * _32
		-	 (_11 * _23 * _32	+	_12 * _21 * _33		+	_13 * _22 * _31);*/

	out._11 *= det;
	out._12 *= det;
	out._13 *= det;
	out._14 *= det;

	out._21 *= det;
	out._22 *= det;
	out._23 *= det;
	out._24 *= det;

	out._31 *= det;
	out._32 *= det;
	out._33 *= det;
	out._34 *= det;

	out._41 *= det;
	out._42 *= det;
	out._43 *= det;
	out._44 *= det;

	return out;
}

template <typename _Type>
const Matrix4<_Type>& Matrix4<_Type>::Translate(const Vec3<_Type> &displacement)
{
	_41 += displacement.x;
	_42 += displacement.y;
	_43 += displacement.z;

	return *this;
}

template <typename _Type>
const Matrix4<_Type>& Matrix4<_Type>::LookAlong(const Vec3<_Type> &v)		// where up is y positive
{
	Vec3<_Type> up = Vec3<_Type>(0.0f, 1.0f, 0.0f);
	Vec3<_Type> z = v.GetNormalized();
	Vec3<_Type> x = up.Cross(z);
	Vec3<_Type> y = z.Cross(x).GetNormalized();

	_11 = x.x;	_12 = x.y;	_13 = x.z;
	_21 = y.x;	_22 = y.y;	_23 = y.z;
	_31 = z.x;	_32 = z.y;	_33 = z.z;

	return *this;
}

template <typename _Type>
_Type Matrix4<_Type>::Determinant() const
{
	return
	_41 * _32  * _23  * _14   -  _31 * _42 * _23  * _14   -
	_41 * _22  * _33 * _14   +  _21 * _42 * _33 * _14   +
	_31  * _22  * _43 * _14   -  _21 * _32  * _43 * _14   -
	_41 * _32  * _13  * _24   +  _31 * _42 * _13  * _24   +
	_41 * _12  * _33 * _24   -  _11 * _42 * _33 * _24   -
	_31  * _12  * _43 * _24   +  _11 * _32  * _43 * _24   +
	_41 * _22  * _13  * _34  -  _21 * _42 * _13  * _34  -
	_41 * _12  * _23  * _34  +  _11 * _42 * _23  * _34  +
	_21  * _12  * _43 * _34  -  _11 * _22  * _43 * _34  -
	_31  * _22  * _13  * _44  +  _21 * _32  * _13  * _44  +
	_31  * _12  * _23  * _44  -  _11 * _32  * _23  * _44  -
	_21  * _12  * _33 * _44  +  _11 * _22  * _33 * _44;
}

template <typename _Type>
const Matrix4<_Type>& Matrix4<_Type>::Transpose()
{
	Matrix4<_Type> temp = *this;

	m[0][0] = temp.m[0][0];
	m[1][0] = temp.m[0][1];
	m[2][0] = temp.m[0][2];
	m[3][0] = temp.m[0][3];

	m[0][1] = temp.m[1][0];
	m[1][1] = temp.m[1][1];
	m[2][1] = temp.m[1][2];
	m[3][1] = temp.m[1][3];

	m[0][2] = temp.m[2][0];
	m[1][2] = temp.m[2][1];
	m[2][2] = temp.m[2][2];
	m[3][2] = temp.m[2][3];

	m[0][3] = temp.m[3][0];
	m[1][3] = temp.m[3][1];
	m[2][3] = temp.m[3][2];
	m[3][3] = temp.m[3][3];

	return *this;
}

template <typename _Type>
Vec3<_Type> Matrix4<_Type>::TransformInverse(const Vec3<_Type> &vector) const
{
	Vec3<_Type> temp = vector;
	temp.x -= _41;
	temp.y -= _42;
	temp.z -= _43;

	return Vec3<_Type>(
		temp.x * _11 +
		temp.y * _21 +
		temp.z * _31,

		temp.x * _12 +
		temp.y * _22 +
		temp.z * _32,

		temp.x * _13 +
		temp.y * _23 +
		temp.z * _33
		);
}

template <typename _Type>
Vec3<_Type> Matrix4<_Type>::Transform(const Vec3<_Type> &vector) const
{
	return vector * *this;
}

template <typename _Type>
Vec3<_Type> Matrix4<_Type>::GetZDir() const
{
	return Vec3<_Type>(_31, _32, _33);
}

template <typename _Type>
Vec3<_Type> Matrix4<_Type>::GetPos() const
{
	return Vec3<_Type>(_41, _42, _43);
}

template <typename _Type>
Matrix3<_Type> Matrix4<_Type>::GetOrientation() const
{
	return m3(	_11, _12, _13,
				_21, _22, _23,
				_31, _32, _33);
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateIdentity()
{
	Matrix4<_Type> out;
	out._11 = 1.0f;		out._12 = 0.0f;		out._13 = 0.0f;		out._14 = 0.0f;
	out._21 = 0.0f;		out._22 = 1.0f;		out._23 = 0.0f;		out._24 = 0.0f;
	out._31 = 0.0f;		out._32 = 0.0f;		out._33 = 1.0f;		out._34 = 0.0f;
	out._41 = 0.0f;		out._42 = 0.0f;		out._43 = 0.0f;		out._44 = 1.0f;

	return out;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateTranslation(const Vec3<_Type> &pos)
{
	Matrix4<_Type> out;
	out._11 = 1.0f;		out._12 = 0.0f;		out._13 = 0.0f;		out._14 = 0.0f;
	out._21 = 0.0f;		out._22 = 1.0f;		out._23 = 0.0f;		out._24 = 0.0f;
	out._31 = 0.0f;		out._32 = 0.0f;		out._33 = 1.0f;		out._34 = 0.0f;
	out._41 = pos.x;	out._42 = pos.y;	out._43 = pos.z;	out._44 = 1.0f;

	return out;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateRotateX(const float a)
{
	Matrix4<_Type> out;
	out._11 = 1.0f;		out._12 = 0.0f;		out._13 = 0.0f;		out._14 = 0.0f;
	out._21 = 0.0f;		out._22 = cos(a);	out._23 = -sin(a);	out._24 = 0.0f;
	out._31 = 0.0f;		out._32 = sin(a);	out._33 = cos(a);	out._34 = 0.0f;
	out._41 = 0.0f;		out._42 = 0.0f;		out._43 = 0.0f;		out._44 = 1.0f;

	return out;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateRotateY(const float a)
{
	Matrix4<_Type> out;
	out._11 = cos(a);	out._12 = 0.0f;		out._13 = sin(a);	out._14 = 0.0f;
	out._21 = 0.0f;		out._22 = 1.0f;		out._23 = 0.0f;		out._24 = 0.0f;
	out._31 = -sin(a);	out._32 = 0.0f;		out._33 = cos(a);	out._34 = 0.0f;
	out._41 = 0.0f;		out._42 = 0.0f;		out._43 = 0.0f;		out._44 = 1.0f;

	return out;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateRotateZ(const float a)
{
	Matrix4<_Type> out;
	out._11 = cos(a);	out._12 = -sin(a);	out._13 = 0.0f;		out._14 = 0.0f;
	out._21 = sin(a);	out._22 = cos(a);	out._23 = 0.0f;		out._24 = 0.0f;
	out._31 = 0.0f;		out._32 = 0.0f;		out._33 = 1.0f;		out._34 = 0.0f;
	out._41 = 0.0f;		out._42 = 0.0f;		out._43 = 0.0f;		out._44 = 1.0f;

	return out;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateRotationFrom(const Quat4<_Type> &q)
{
	Matrix4<_Type> out;
	out._11 = 1 - 2 * (q.y * q.y + q.z * q.z);
	out._12 = 2 * (q.x * q.y - q.z * q.w);
	out._13 = 2 * (q.x * q.z + q.y * q.w);

	out._21 = 2 * (q.x * q.y + q.z * q.w);
	out._22 = 1 - 2 * (q.x * q.x + q.z * q.z);
	out._23 = 2 * (q.y * q.z - q.x * q.w);

	out._31 = 2 * (q.x * q.z - q.y * q.w);
	out._32 = 2 * (q.y * q.z + q.x * q.w);
	out._33 = 1 - 2 * (q.x * q.x + q.y * q.y);

	out._14 = 0.0f;
	out._24 = 0.0f;
	out._34 = 0.0f;

	out._41 = 0.0f;
	out._42 = 0.0f;
	out._43 = 0.0f;
	out._44 = 1.0f;

	return out;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateOrthographicLH(const float width, const float height, const float zNear, const float zFar)
{
	Matrix4<_Type> m;
	m._11 = 2.0f / width;
	m._22 = 2.0f / height;
	m._33 = 1.0f / (zFar - zNear);
	m._43 = zNear / (zNear - zFar);
	m._44 = 1.0f;

	m._12 = m._13 = m._14 =
		m._21 = m._23 = m._24 =
		m._31 = m._32 = m._34 =
		m._41 = m._42 = 0.0f;

	return m;
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateYawPitchRoll(const float yaw, const float pitch, const float roll)
{
	return CreateRotateZ(roll) * CreateRotateX(pitch) * CreateRotateY(yaw);
}

template <typename _Type>
Matrix4<_Type> Matrix4<_Type>::CreateScale(const Vec3<_Type> &in)
{
	Matrix4<_Type> m;

	m._11 = in.x;		m._12 = 0.0f;		m._13 = 0.0f;		m._14 = 0.0f;
	m._21 = 0.0f;		m._22 = in.y;		m._23 = 0.0f;		m._24 = 0.0f;
	m._31 = 0.0f;		m._32 = 0.0f;		m._33 = in.z;		m._34 = 0.0f;
	m._41 = 0.0f;		m._42 = 0.0f;		m._43 = 0.0f;		m._44 = 1.0f;

	return m;
}

template <typename _Type>
const Matrix4<_Type> Matrix4<_Type>::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
													0.0f, 1.0f, 0.0f, 0.0f,
													0.0f, 0.0f, 1.0f, 0.0f,
													0.0f, 0.0f, 0.0f, 1.0f);

/*==================
Quat4<_Type>
==================*/

template <typename _Type>
Quat4<_Type>::Quat4(const _Type w, const _Type x, const _Type y, const _Type z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

template <typename _Type>
Quat4<_Type>& Quat4<_Type>::operator=(const Quat4<_Type>& in)
{
	q[0] = in.q[0];
	q[1] = in.q[1];
	q[2] = in.q[2];
	q[3] = in.q[3];

	return *this;
}

template <typename _Type>
void Quat4<_Type>::operator*=(const Quat4<_Type>& in)
{
	Quat4<_Type> q = *this;

	w = q.w * in.w - q.x * in.x -
		q.y * in.y - q.z * in.z;

	x = q.w * in.x + q.x * in.w +
		q.y * in.z - q.z * in.y;

	y = q.w * in.y + q.y * in.w +
		q.z * in.x - q.x * in.z;

	z = q.w * in.z + q.z * in.w +
		q.x * in.y - q.y * in.x;
}

template <typename _Type>
Quat4<_Type> Quat4<_Type>::operator*(const Quat4<_Type> &in)
{
	Quat4<_Type> out;

	out.w = w * in.w - x * in.x -
		y * in.y - z * in.z;

	out.x = w * in.x + x * in.w +
		y * in.z - z * in.y;

	out.y = w * in.y + y * in.w +
		z * in.x - x * in.z;

	out.z = w * in.z + z * in.w +
		x * in.y - y * in.x;

	return out;
}

template <typename _Type>
Vec3<_Type>	 Quat4<_Type>::operator*(const Vec3<_Type> &in)
{
	Quat4<_Type> temp = GetNormalized();
	Quat4<_Type> vec;
	vec.x = in.x;
	vec.y = in.y;
	vec.z = in.z;
	vec.w = 1.0f;

	vec = temp * vec * temp.GetConjugate();

	return v3(vec.x, vec.y, vec.z);
}

template <typename _Type>
const Quat4<_Type>& Quat4<_Type>::AddScaledVector(const Vec3<_Type> &in, const _Type scale)
{
	Quat4<_Type> q(0.0f,
		in.x * scale,
		in.y * scale,
		in.z * scale);

	q *= *this;	
	w += q.w * 0.5f;
	x += q.x * 0.5f;
	y += q.y * 0.5f;
	z += q.z * 0.5f;

	return *this;
}

template <typename _Type>
void Quat4<_Type>::RotateByVector(const Vec3<_Type> &in)
{
	Quat4<_Type> q(0.0f, in.x, in.y, in.z);
	(*this) *= q;
}

template <typename _Type>
void Quat4<_Type>::Normalize()
{
	float d = MagnitudeSquared();

	if(d == 0.0f)
	{
		w = 1.0f;
		return;
	}

	d = 1.0f / sqrtf(d);
	w *= d;
	x *= d;
	y *= d;
	z *= d;
}

template <typename _Type>
Quat4<_Type> Quat4<_Type>::GetNormalized()
{
	Quat4<_Type> out;

	float d = MagnitudeSquared();

	if(d == 0.0f)
	{
		out.x = out.y = out.z = 0.0f;
		out.w = 1.0f;
		return out;
	}

	d = 1.0f / sqrtf(d);
	out.w = w * d;
	out.x = x * d;
	out.y = y * d;
	out.z = z * d;

	return out;
}

template <typename _Type>
float Quat4<_Type>::MagnitudeSquared()
{
	return  w * w + x * x + y * y + z * z;
}

template <typename _Type>
void Quat4<_Type>::Conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

template <typename _Type>
Quat4<_Type> Quat4<_Type>::GetConjugate()
{
	Quat4<_Type> out;
	out.w = w;
	out.x = -x;
	out.y = -y;
	out.z = -z;

	return out;
}

template <typename _Type>
Quat4<_Type> Quat4<_Type>::SetOrientation(const Vec3<_Type> &axis, const _Type radians)
{
	Quat4<_Type> ori;
	float sinA = sinf(radians * 0.5f);

	ori.w = cosf(radians * 0.5f);
	ori.x = axis.x * sinA;
	ori.y = axis.y * sinA;
	ori.z = axis.z * sinA;

	return ori;
}