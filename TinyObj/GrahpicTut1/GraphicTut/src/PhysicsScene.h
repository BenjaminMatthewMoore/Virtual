#pragma once
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd\PxVisualDebugger.h>

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void SetUpPhysx();

	PxFoundation* g_physicsFoundation;
	PxPhysics* g_physics;
	PxScene* g_physicsScene;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;

private:
	
};

