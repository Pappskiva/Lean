#include "PhysicsManager.h"

m4 PhysicsManager::ConvertBulletTransform(btTransform *bulletTransformMatrix)
{
	m4 world;
	btVector3 R = bulletTransformMatrix->getBasis().getColumn(0);
	btVector3 U = bulletTransformMatrix->getBasis().getColumn(1);
	btVector3 L = bulletTransformMatrix->getBasis().getColumn(2);
	btVector3 P = bulletTransformMatrix->getOrigin();

	v3 vR, vU, vL, vP;
	vR.x = R.x(); vR.y = R.y(); vR.z = R.z();
	vU.x = U.x(); vU.y = U.y(); vU.z = U.z();
	vL.x = L.x(); vL.y = L.y(); vL.z = L.z();
	vP.x = P.x(); vP.y = P.y(); vP.z = P.z();

	PrepareMatrixFromRULP(world, &vR, &vU, &vL, &vP);
	return world;
}
btTransform PhysicsManager::ConvertD3DXMatrix(m4 *d3dMatrix)
{
	btTransform bulletTransformMatrix;
	btVector3 R, U, L, P;
	R.setX(d3dMatrix->_11); R.setY(d3dMatrix->_12); R.setZ(d3dMatrix->_13);
	U.setX(d3dMatrix->_21); U.setY(d3dMatrix->_22); U.setZ(d3dMatrix->_23);
	L.setX(d3dMatrix->_31); L.setY(d3dMatrix->_32); L.setZ(d3dMatrix->_33);
	P.setX(d3dMatrix->_41); P.setY(d3dMatrix->_42); P.setZ(d3dMatrix->_43);

	bulletTransformMatrix.getBasis().setValue(R.x(), U.x(), L.x(),
		R.y(), U.y(), L.y(),
		R.z(), U.z(), L.z());
	bulletTransformMatrix.setOrigin(P);
	return bulletTransformMatrix;
}

void PhysicsManager::PrepareMatrixFromRULP(m4 &matOutput, v3 *R, v3 *U, v3 *L, v3 *P)
{
	matOutput._11 = R->x; matOutput._12 = R->y; matOutput._13 = R->z; matOutput._14 = 0.f;
	matOutput._21 = U->x; matOutput._22 = U->y; matOutput._23 = U->z; matOutput._24 = 0.f;
	matOutput._31 = L->x; matOutput._32 = L->y; matOutput._33 = L->z; matOutput._34 = 0.f;
	matOutput._41 = P->x; matOutput._42 = P->y; matOutput._43 = P->z; matOutput._44 = 1.f;
}
