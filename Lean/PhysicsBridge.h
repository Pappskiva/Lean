////////////////////////////////////////////////////////////////////////////////
// Filename:  PhysicsBridge.h
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"
#include "PhysicsManager.h"


#include "Bullet\src\BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"
#include "Heightfield.h"
#include "Ball.h"
#include "Level.h"


class PhysicsBridge
{
public:
	PhysicsBridge();
	~PhysicsBridge();

	void Initialize(Level* level, Ball* ball);
	void StepSimulation(float deltaTime,
		float rotX, float rotY, float rotZ,
		Ball* ball, Level* level);
	m4 GetBallTransformMatrix();
	m4 GetPlaneTransformMatrix();

	void ReInitialize(Level* level, Ball* ball);
	void ActivateBall();

	void DebugRender(D3D* d3d);
	void GenerateDebug(D3D* d3d);
	void ResetBall(Ball* ball);

	float* heightMap;

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	// Ground
	btCollisionShape* groundShape;
	btDefaultMotionState* groundMotionState;
	btRigidBody* groundRigidBody;

	//Heightmap
	Heightfield* heightmapShape;
	btDefaultMotionState* heightmapMotionState;
	btRigidBody* heightmapRigidBody;

	int m_HeightStickWidth;   //Bredd på kartan // must be (2^N) + 1
	int m_HeightStickLeangth; //Längd på kartan // must be (2^N) + 1
	const void* m_HeightMapData;//  = getRawHeightfieldData(m_model, m_type, m_minHeight, m_maxHeight);
	btScalar m_HeightScale;//Höjd skala
	btScalar m_MinHeight;//
	btScalar m_MaxHeight;//
	int m_UpAxis;		//		// start with Y-axis as "up"
	PHY_ScalarType m_HeightDataType;//
	bool m_FlipQuadEdges;//

	// Ball
	btCollisionShape* fallShape;
	btDefaultMotionState* fallMotionState;
	btRigidBody* fallRigidBody;
	
	btScalar mass;
	btVector3 fallInertia;
};

