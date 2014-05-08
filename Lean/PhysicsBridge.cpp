////////////////////////////////////////////////////////////////////////////////
// Filename: PhysicsBridge.cpp
////////////////////////////////////////////////////////////////////////////////

#include "PhysicsBridge.h"
#include "D3D.h"
#define GRAVITY -100

PhysicsBridge::PhysicsBridge(){}
PhysicsBridge::~PhysicsBridge()
{
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;


	dynamicsWorld->removeRigidBody(heightmapRigidBody);
	delete heightmapRigidBody->getMotionState();
	delete heightmapRigidBody;

	delete fallShape;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}
void PhysicsBridge::Initialize(Level* level)
{
	heightMap = level->GetHeighMapData();
	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));

	//HeightMap
	m_HeightStickWidth = 64;   //Bredd p� kartan // must be (2^N) + 1
	m_HeightStickLeangth = 64; //L�ngd p� kartan // must be (2^N) + 1
	m_HeightMapData = heightMap;// getRawHeightfieldData(m_model, m_type, m_minHeight, m_maxHeight);
	m_HeightScale = 1;//H�jd skala
	m_MinHeight = -255;//
	m_MaxHeight = 255;//
	m_UpAxis = 1;		//		// start with Y-axis as "up"
	m_HeightDataType = PHY_FLOAT;//

	m_FlipQuadEdges = false;

	heightmapShape = new Heightfield(m_HeightStickWidth, m_HeightStickLeangth, m_HeightMapData, m_HeightScale, m_MinHeight, m_MaxHeight, m_UpAxis, m_HeightDataType, m_FlipQuadEdges);
	heightmapShape->setLocalScaling(btVector3(1.0f, 2.5f, 1.0f));
	heightmapMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo heightmapRigidBodyCI(0, heightmapMotionState, heightmapShape, btVector3(0, 0, 0));
	heightmapRigidBody = new btRigidBody(heightmapRigidBodyCI);
	dynamicsWorld->addRigidBody(heightmapRigidBody);

	// Ball
	fallShape = new btSphereShape(1);
	//fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-2, 10, 5)));
	btScalar mass = 2;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
}

void PhysicsBridge::StepSimulation(float deltaTime, float rotX, float rotY, float rotZ, Ball* ball, Level* level)
{
	btVector3 newGravity = btVector3(0, GRAVITY, 0);

	newGravity = newGravity.rotate(btVector3(1, 0, 0), rotX); // Rot X
	newGravity = newGravity.rotate(btVector3(0, 1, 0), rotY); // Rot Y
	newGravity = newGravity.rotate(btVector3(0, 0, 1), rotZ); // Rot Z
	dynamicsWorld->setGravity(newGravity);
	dynamicsWorld->stepSimulation(deltaTime);

	//Ger bollen r�tt v�rldsmatris s� att fysiken funkar
	m4 world = GetBallTransformMatrix();
	m4 rotationL = m4::CreateYawPitchRoll(0, rotX, rotZ);
	world = world*rotationL;
	ball->SetWorldMatrix(world);

	world = GetPlaneTransformMatrix();
	m4 rotationMatrix = m4::CreateYawPitchRoll(rotY, rotX, rotZ);
	world = rotationMatrix * world;
	level->SetWorldMatrix(world);
}

m4 PhysicsBridge::GetBallTransformMatrix()
{
	btTransform trans;
	fallMotionState->getWorldTransform(trans);
	return PhysicsManager::ConvertBulletTransform(&trans);
}

m4 PhysicsBridge::GetPlaneTransformMatrix()
{
	btTransform trans;
	heightmapMotionState->getWorldTransform(trans);
	return PhysicsManager::ConvertBulletTransform(&trans);

}
void PhysicsBridge::DebugRender(D3D* d3d)
{
	d3d->RenderMesh(heightmapShape->GetDebugMesh());
}

void PhysicsBridge::GenerateDebug(D3D* d3d)
{
	heightmapShape->GenerateDebugHeightmap(d3d);
}