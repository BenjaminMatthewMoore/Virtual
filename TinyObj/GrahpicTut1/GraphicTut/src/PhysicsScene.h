#pragma once
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd\PxVisualDebugger.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

using namespace physx;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void SetUpPhysx();
	
	void DrawScene();

	PxFoundation* g_physicsFoundation;
	PxPhysics* g_physics;
	PxScene* g_physicsScene;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;

private:
	void AddWidget(physx::PxShape* shape, physx::PxRigidActor* actor, glm::vec4 geo_color);
	physx::PxScene* m_pScene;
};

