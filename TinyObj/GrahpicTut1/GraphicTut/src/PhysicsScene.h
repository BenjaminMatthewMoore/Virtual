#pragma once
#include <PxPhysicsAPI.h>
#include "ControllerHitReport.h"
#include <pvd\PxVisualDebugger.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

class PlayerController;
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void SetUpPhysx();
	
	void DrawScene();

	void Update(float a_deltaTime);

	
	
	
	
	

private:
	void AddWidget(physx::PxShape* shape, physx::PxRigidActor* actor, glm::vec4 geo_color);
	physx::PxScene* m_pScene;
	PlayerController* m_playerController;
	ControllerHitReport* hitReport;
	float m_gravity;
	physx::PxFoundation* m_physicsFoundation;
	physx::PxPhysics* m_physics;
	physx::PxScene* m_physicsScene;
	physx::PxMaterial* m_physicsMaterial;
	physx::PxMaterial* m_boxMaterial;
	physx::PxCooking* m_physicsCooker;

	physx::PxDefaultErrorCallback mDefaultErrorCallback;
	physx::PxDefaultAllocator mDefaultAllocatorCallback;

};

