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


	if (heightmapRigidBody)
	{
		dynamicsWorld->removeRigidBody(heightmapRigidBody);
		delete heightmapRigidBody->getMotionState();
		delete heightmapRigidBody;
	}

	delete fallShape;
	delete heightmapShape;
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}
void PhysicsBridge::Initialize(Level* level, Ball* ball)
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
	m_HeightStickWidth = level->GetWidth();   //Bredd p� kartan // must be (2^N) + 1
	m_HeightStickLeangth = level->GetLength(); //L�ngd p� kartan // must be (2^N) + 1
	m_HeightMapData = heightMap;// getRawHeightfieldData(m_model, m_type, m_minHeight, m_maxHeight);
	m_HeightScale = 1;//H�jd skala
	m_MinHeight = -255;//
	m_MaxHeight = 255;//
	m_UpAxis = 1;		//		// start with Y-axis as "up"
	m_HeightDataType = PHY_FLOAT;//

	m_FlipQuadEdges = false;

	heightmapShape = new Heightfield(m_HeightStickWidth, m_HeightStickLeangth, m_HeightMapData, m_HeightScale, m_MinHeight, m_MaxHeight, m_UpAxis, m_HeightDataType, m_FlipQuadEdges);
	heightmapShape->setLocalScaling(btVector3(level->GetScale(), level->GetMaxHeight(), level->GetScale()));
	heightmapMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo heightmapRigidBodyCI(0, heightmapMotionState, heightmapShape, btVector3(0, 0, 0));
	heightmapRigidBody = new btRigidBody(heightmapRigidBodyCI);
	dynamicsWorld->addRigidBody(heightmapRigidBody);

	// Ball
	fallShape = new btSphereShape(1);
	//fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
	v3 ballStartPos;
	ball->GetPosition(ballStartPos);
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(ballStartPos.x, ballStartPos.y, ballStartPos.z)));
	mass = 2;
	fallInertia = btVector3(9, 9, 9);
	//fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);

	fallRigidBody->setRollingFriction(0.02);
}

void PhysicsBridge::ReInitialize(Level* level, Ball* ball)
{
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	if (heightmapRigidBody)
	{
		dynamicsWorld->removeRigidBody(heightmapRigidBody);
		delete heightmapRigidBody->getMotionState();
		delete heightmapRigidBody;
	}
	

	delete fallShape;
	delete heightmapShape;
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;


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
	m_HeightStickWidth = level->GetWidth();   //Bredd p� kartan // must be (2^N) + 1
	m_HeightStickLeangth = level->GetLength(); //L�ngd p� kartan // must be (2^N) + 1
	m_HeightMapData = heightMap;// getRawHeightfieldData(m_model, m_type, m_minHeight, m_maxHeight);
	m_HeightScale = 1;//H�jd skala
	m_MinHeight = -255;//
	m_MaxHeight = 255;//
	m_UpAxis = 1;		//		// start with Y-axis as "up"
	m_HeightDataType = PHY_FLOAT;//

	m_FlipQuadEdges = false;

	heightmapShape = new Heightfield(m_HeightStickWidth, m_HeightStickLeangth, m_HeightMapData, m_HeightScale, m_MinHeight, m_MaxHeight, m_UpAxis, m_HeightDataType, m_FlipQuadEdges);
	heightmapShape->setLocalScaling(btVector3(level->GetScale(), level->GetMaxHeight(), level->GetScale()));
	heightmapMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo heightmapRigidBodyCI(0, heightmapMotionState, heightmapShape, btVector3(0, 0, 0));
	heightmapRigidBody = new btRigidBody(heightmapRigidBodyCI);
	dynamicsWorld->addRigidBody(heightmapRigidBody);

	// Ball
	fallShape = new btSphereShape(1);
	//fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
	v3 ballStartPos;
	ball->GetPosition(ballStartPos);
	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(ballStartPos.x, ballStartPos.y, ballStartPos.z)));
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
}

void PhysicsBridge::StepSimulation(float deltaTime, float rotX, float rotY, float rotZ, Ball* ball, Level* level)
{
	// Uppdatera bollens halhet
	fallRigidBody->setFriction(ball->GetFriction());
	v3 forceToApply = ball->GetAndZeroAddedForce();
	if (forceToApply != 0)
	{
		fallRigidBody->applyCentralImpulse(btVector3(forceToApply.x, forceToApply.y, forceToApply.z));
	}

	// Gravitation
	btVector3 newGravity = btVector3(0, GRAVITY, 0);

	newGravity = newGravity.rotate(btVector3(1, 0, 0), rotX); // Rot X
	newGravity = newGravity.rotate(btVector3(0, 1, 0), rotY); // Rot Y
	newGravity = newGravity.rotate(btVector3(0, 0, 1), rotZ); // Rot Z
	dynamicsWorld->setGravity(newGravity);
	dynamicsWorld->stepSimulation(deltaTime);

	//Ger bollen r�tt v�rldsmatris s� att fysiken funkar
	m4 world = GetBallTransformMatrix();

	// Spara bollens position innan bollens position har roterats
	v3 newBallPosition = world.GetPos(); 
	ball->SetFlatPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);

	m4 rotationL = m4::CreateYawPitchRoll(0, rotX, rotZ);
	world = world*rotationL;
	ball->SetWorldMatrix(world);
	
	//Tillagd f�r att man ska kunna f� ut r�tt position direkt fr�n bollen
	newBallPosition = world.GetPos();
	ball->SetPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);

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

void PhysicsBridge::ActivateBall()
{
	fallRigidBody->activate();
}

void PhysicsBridge::ResetBall(Ball* ball)
{
	btVector3 startPos = fallMotionState->m_startWorldTrans.getOrigin();
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startPos));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
}