#ifndef TRACETESTS_H

#include "DMath.h"

/*=============================================================================

	Basic primitives

=============================================================================*/

struct Ray
{
	v3		pos;
	v3		dir;
};

struct Ray2D
{
	v2		pos;
	v2		dir;
};

struct Line
{
	v3		p1;
	v3		p2;
};

struct Circle
{
	v2		center;
	float	radius;
};

struct Sphere
{
	v3		pos;
	float	radius;
};

struct Ellipsoid
{
	v3		pos;
	float	radiusX;
	float	radiusY;
};

struct AABB
{
	v3		pos;
	v3		extents;
};

struct OBB
{
	v3 center;
	v3 lengths;
	v3 uvw[3];

	OBB(){}

	OBB(const v3 center, const v3 lengths, const v3 up, const v3 side)
	{
		this->center = center;
		this->lengths = lengths;

		uvw[0] = side;
		uvw[0].Normalize();

		uvw[1] = up;
		uvw[1].Normalize();

		uvw[2] = uvw[0].Cross(uvw[1]);
		uvw[2].Normalize();
	}
};

struct Plane
{
	float	d;
	v3		normal;
};

struct Triangle
{
	v3		points[3];
};

struct Quad
{
	v3		points[4];
};

struct Capsule
{
	v3		p1;
	v3		p2;
	float	radius;
};

float			RayVsPlane(const Ray& r, const Plane& p);
float			RayVsSphere(const Ray& r, const Sphere& s);
float			RayVsTriangle(const Ray& r, const Triangle& t);
float			RayVsBox(const Ray& r, const OBB& b);

float			DistPointSphere(const v3 &p, const Sphere &s);
float			DistSphereSphere(const Sphere &s1, const Sphere &s2);
float			DistLineTriangle(const Line &line, const Triangle &tri);	// needed?

v3				CalculateNormalFromTriangle(const Triangle &triangle);

v3				ClosestPointLinePoint(const Line &line, const v3 &point);
v3				ClosestPointLineTriangle(const Line &line, const Triangle &tri);

float			SphereSphereIntersection(const Sphere &s1, const Sphere &s2);

bool			PointInTriangle(const Triangle &triangle, const v3 &point);

bool			TestSphereSphere(const Sphere &s1, const Sphere &s2);
bool			TestSphereCapsule(const Sphere &s, const Capsule &c);

bool			TraceRayQuad(const Ray &ray, const Quad &quad, float &t);
bool			TraceRaySphere(const Ray &ray, const Sphere &sphere, float &t);
bool			TraceRayPlane(const Ray &ray, const Plane &plane, float &t);
bool			TraceSphereSphere(const Sphere &s1, const Sphere &s2, const v3 &v1, const v3 &v2, float &t);
bool			TraceSpherePlane(const v3 &v1, const Sphere &sphere, const Plane &plane, float &t);
bool			TraceSphereTriangle(const v3 &v1, const Sphere &sphere, const Triangle &tri, float &t);

class IntersectionTest
{
public:
	static bool RayCircle(const Ray2D &ray, const Circle &circle);
	static bool CircleCircle(const Circle &c1, const Circle &c2);
};

#define TRACETESTS_H
#endif