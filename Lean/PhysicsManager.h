#pragma once
#include <btBulletDynamicsCommon.h>
#include "D3D Includes.h"
#include "DuckRenderer\DMath.h"

class PhysicsManager
{
public:
	static m4 ConvertBulletTransform(btTransform *bulletTransformMatrix);
	static m4 ConvertBulletTransformPlane(btTransform *bulletTransformMatrix);
	static btTransform ConvertD3DXMatrix(m4 *d3dMatrix);

private:
	static void PrepareMatrixFromRULP(m4 &matOutput, v3 *R, v3 *U, v3 *L, v3 *P);
};