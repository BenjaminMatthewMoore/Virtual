#pragma once
#include <PxPhysicsAPI.h>
#include "ControllerHitReport.h"
#include <pvd\PxVisualDebugger.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

class PlayerController;
class TerrainGen;
class Camera;
class Window;
class FluidParticles;
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void SetUpPhysx(float TerrainSize);
	
	void DrawScene();

	void Update(float a_deltaTime, Window* window);
	
	void CreateHeightMap(TerrainGen terrain);
	void setUpVisualDebugger();
	
	void ShootCubes(Camera* camera);
	
	physx::PxFoundation* m_physicsFoundation;
	physx::PxPhysics* m_physics;
	physx::PxScene* m_physicsScene;
	physx::PxMaterial* m_physicsMaterial;
	physx::PxMaterial* m_boxMaterial;
	physx::PxCooking* m_physicsCooker;
	void AddWidget(physx::PxShape* shape, physx::PxRigidActor* actor, glm::vec4 geo_color);
	
	FluidParticles* fluidSystem;

private:
	
	//physx::PxScene* m_pScene;
	PlayerController* m_playerController;
	ControllerHitReport* m_hitReport;
	float m_gravity;


	physx::PxDefaultErrorCallback* mDefaultErrorCallback;
	physx::PxAllocatorCallback* mDefaultAllocatorCallback;
	physx::PxSimulationFilterShader m_DefualtFilterShader = PxDefaultSimulationFilterShader;
	physx::PxRigidActor* m_pXActor;

};

