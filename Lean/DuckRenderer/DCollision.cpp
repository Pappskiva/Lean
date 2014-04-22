#include "DCollision.h"

float RayVsPlane(const Ray& r, const Plane& p)
{
	return (p.d - (r.pos * p.normal)) / (r.dir * p.normal);
}

float RayVsSphere(const Ray& r, const Sphere& s)
{
	v3 vect = s.pos - r.pos;
	float lenSquared = powf(vect.Length(), 2.0f);
	float radiusSquared = s.radius * s.radius;
	float dot = vect * r.dir;
	float t = -1.0f;
	bool hit = false;
	
	if(hit = (dot >= 0.0f && !(lenSquared < radiusSquared) ))
	{
		float m2 = lenSquared - powf(dot, 2.0f);
		hit = m2 <= radiusSquared;

		if(hit)
		{
			float root = sqrtf(radiusSquared - m2);
			t = dot - root;
			float t2 = dot + root;

			if(t2 < t)
				t = t2;
		}
	}

	if(!hit)
		t = -1.0f;

	return t;
}

float RayVsTriangle(const Ray& r, const Triangle& t)
{
	v3 e1(t.points[1] - t.points[0]), e2(t.points[2] - t.points[0]), s(r.pos - t.points[0]);	
	float det = m3(-r.dir, e1, e2).Determinant();
	float out = -1.0f;

	if(det != 0)
	{
		float inv = 1.0f / det;
		float time = m3(s, e1, e2).Determinant() * inv;
		float u = m3(-r.dir, s, e2).Determinant() * inv;
		float v = m3(-r.dir, e1, s).Determinant() * inv;
	
		if(u > 0 && v > 0 && u + v < 1)
			out = time;
	}


	return out;
}

float RayVsBox(const Ray& r, const OBB& b)
{
	float t_min = 1.175494351e-38F;
	float t_max = 3.402823466e+38F;
	v3 p = b.center - r.pos;
	float t = -1.0f;

	for(unsigned int i = 0; i < 3; ++i)
	{
		float e = b.uvw[i] * p;
		v3 f = b.uvw[i] * r.dir;

		if(f.Length() > (10^-20) )
		{
			float t1 = (e + b.lengths.v[i]) / f.Length();
			float t2 = (e - b.lengths.v[i]) / f.Length();
			
			if(t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if(t1 > t_min)
				t_min = t1;

			if(t2 < t_max)
				t_max = t2;

			if(t_min > t_max)
				return t;
			
			if(t_max < 0.0f)
				return t;
		}
		else if(-e -b.lengths.v[i] > 0 || -e + b.lengths.v[i] < 0.0f)
			return t;
	}
	if(t_min > 0.0f)
	{
		t = t_min;
		return t;
	}
	else
		t = t_max;

	return t;
}

float DistPointSphere(const v3 &p, const Sphere &s)
{
	float length = (p - s.pos).Length();
	
	return length - s.radius;
}

float DistSphereSphere(const Sphere &s1, const Sphere &s2)
{
	float length = (s1.pos - s2.pos).Length();
	float r = s1.radius + s2.radius;
	float bajs = length - r;

	if(bajs < 0.0f)
		bajs = 0.0f;

	return bajs;
}

v3	CalculateNormalFromTriangle(const Triangle &triangle)
{
	return (triangle.points[1] - triangle.points[0]).Cross(triangle.points[2] - triangle.points[0]).GetNormalized();
}

v3 ClosestPointLinePoint(const Line &line, const v3 &point)
{
	v3 p1p2 = (line.p2 - line.p1).GetNormalized();
	v3 p1p = point - line.p1;
	float t = p1p2 * p1p;
	float lengthSquare = (line.p2 - line.p1).LengthSquare();

	if(t < 0.0f)
		return line.p1;
	if(t * t > lengthSquare)
		return line.p2;

	return v3(line.p1 + p1p2 * t);
}

v3 ClosestPointLineTriangle(const Line &line, const Triangle &tri)	// get t too? to calculate push length
{
	v3 normal = (tri.points[1] - tri.points[0]).Cross(tri.points[2] - tri.points[0]).GetNormalized();
	float d = normal * tri.points[0];
	v3 dir = line.p2 - line.p1;

	float denominator = normal.x * dir.x + normal.y * dir.y + normal.z * dir.z;

	float numerator = normal.x * line.p1.x + normal.y * line.p1.y +	normal.z * line.p1.z + d;

	if(denominator == 0.0f) // EPSILON?
		if(numerator == 0.0f)
			return 0.0f;
		else
			return 1.0f;

	float t = -numerator / denominator;

	if(t > 1.0f)
		return line.p2;
	if(t < 0.0f)
		return line.p1;

	return line.p1 + (dir * t);
}

bool PointInTriangle(const Triangle &triangle, const v3 &point)
{
	v3 v1, v2, vec;

	v1 = triangle.points[1] - triangle.points[0];
	v2 = triangle.points[2] - triangle.points[0];
	vec = point - triangle.points[0];
										
	float dot00 = v1.Dot(v1);
	float dot01 = v1.Dot(v2);
	float dot02 = v1.Dot(vec);
	float dot11 = v2.Dot(v2);
	float dot12 = v2.Dot(vec);

	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	if((u > 0) && (v > 0) && (u + v < 1))
	{
		return true;
	}

	return false;
}

bool TestSphereSphere(const Sphere &s1, const Sphere &s2)
{
	v3 birdWay = s1.pos - s2.pos;
	float r2 = (s1.radius + s2.radius) * (s1.radius + s2.radius);

	if(birdWay.LengthSquare() < r2)
		return true;

	return false;
}

float SphereSphereIntersection(const Sphere &s1, const Sphere &s2)
{
	float length = (s1.pos - s2.pos).Length();
	float r = s1.radius + s2.radius;
	float bajs = r - length;

	if(bajs < 0.0f)
		bajs = 0.0f;

	return bajs;
}

bool TestSphereCapsule(const Sphere &s, const Capsule &c)
{
	Line line;

	line.p1 = c.p1;
	line.p2 = c.p2;

	v3 closestPoint = ClosestPointLinePoint(line, s.pos);

	float lengthSquare = (s.pos - closestPoint).LengthSquare();
	float r2 = (s.radius + c.radius) * (s.radius + c.radius);

	if(lengthSquare < r2)
		return true;

	return false;
}

bool TraceRayPlane(const Ray &ray, const Plane &plane, float &t)
{
	float denominator = plane.normal.x * ray.dir.x + plane.normal.y * ray.dir.y +
		plane.normal.z * ray.dir.z;

	float numerator = plane.normal.x * ray.pos.x + plane.normal.y * ray.pos.y +
		plane.normal.z * ray.pos.z + plane.d;

	if(denominator == 0.0f) // EPSILON?
		if(numerator == 0.0f)
			return 0.0f;
		else
			return 1.0f;

	t = -numerator / denominator;

	return true;
}

bool TraceRaySphere(const Ray &ray, const Sphere &sphere, float &t)
{
	v3 birdWay = sphere.pos - ray.pos;

	float a = ray.dir * birdWay;
	float b = birdWay * birdWay;
	float r2 = sphere.radius * sphere.radius;

	float discriminant = r2 - b + a * a;

	if(discriminant < 0.0f) // EPSILON?
		return false;		// No collision, move all the way

	t = a - FastSqrt(discriminant);

	return true;
}

bool TraceSphereSphere(const Sphere &s1, const Sphere &s2, const v3 &v1, const v3 &v2, float &t)
{
	v3 birdWay = s2.pos - s1.pos;
	v3 v = v1 - v2;

	float r11 =s1.radius;
	float r21 = s2.radius;
	float r = r11 + r21;
	float a = v * birdWay / v1.Length();	// redo the expression for possible optimization
	float b = birdWay * birdWay;
	float r2 = r * r;

	if(b < r2)
	{
		t = 0.0f;
		return true;
	}

	float discriminant = r2 - b + a * a;

	if(discriminant < 0.0f) // epsilon?
		return false;		// no collision, move all the way

	t = a - FastSqrt(discriminant);

	return true;
}

bool TraceSphereCapsule(const Sphere &s1, const Sphere &s2, const v3 &v1, const v3 &v2, float &t)
{
	// possible?

	return true;
}

bool TraceSpherePlane(v3 &v1, Sphere &sphere, Plane &plane, float &t)
{
	float denominator = plane.normal.x * v1.x + plane.normal.y * v1.y + plane.normal.z * v1.z;

	float numerator = plane.normal.x * sphere.pos.x + plane.normal.y * sphere.pos.y +
		plane.normal.z * sphere.pos.z + plane.d - sphere.radius;

	if(denominator == 0.0f) // EPSILON?
		if(numerator == 0.0f)
			return 0.0f;
		else
			return 1.0f;

	t = -numerator / denominator;

	return true;
}

bool TraceSphereTriangle(const v3 &v1, const Sphere &sphere, const Triangle &tri, float &t)
{
	v3 normal = (tri.points[1] - tri.points[0]).Cross(tri.points[2] - tri.points[0]).GetNormalized();
	float d = tri.points[0] * normal;

	float denominator = normal.x * v1.x + normal.y * v1.y + normal.z * v1.z;

	float numerator = normal.x * sphere.pos.x + normal.y * sphere.pos.y +
		normal.z * sphere.pos.z + d - sphere.radius;

	if(denominator == 0.0f) // EPSILON?
		if(numerator == 0.0f)
			return 0.0f;
		else
			return 1.0f;

	t = -numerator / denominator;

	return true;
}

bool IntersectionTest::RayCircle(const Ray2D &ray, const Circle &circle)
{
	v2 f = ray.pos - circle.center;
	f.Normalize();

	float a = ray.dir * ray.dir;
	float b = 2 * (f * ray.dir);
	float c = f * f - circle.radius * circle.radius;

	float discriminant = b*b-4*a*c;

	if( discriminant == 0.0f )
	  return false;	

	discriminant = sqrt( discriminant );

	float t1 = (-b - discriminant) / (2 * a);
	float t2 = (-b + discriminant) / (2 * a);

	if( t1 >= 0 && t1 <= 1 )
		return true;

	if( t2 >= 0 && t2 <= 1 )
		return true;

	return false;
}

bool IntersectionTest::CircleCircle(const Circle &c1, const Circle &c2)
{
	float dist = (c1.center - c2.center).LengthSquare();
	float combinedRadius = c1.radius + c2.radius;

	if(dist < combinedRadius * combinedRadius)
		return true;

	return false;
}