#pragma once
#include "PxBroadPhase.h"
#include <geometry\PxCapsuleGeometry.h>
#include <characterkinematic\PxExtended.h>
namespace physx {
	class PxController;
	class PxControllerManager;
	class PxCapsuleGeometry;
	struct PxExtendedVec3;
	class PxRigidDynamic;
}
class ControllerHitReport;
class PhysicsScene;
class Window;

class PlayerController
{
public:

	PlayerController(PhysicsScene* physicScene);
	~PlayerController();
	void Update(float deltaTime, Window* activeWindow);
private:
	bool onGround;
	float movementSpeed;
	float roatationSpeed;
	float m_velocity;
	float m_rotation;
	float m_gravity;

	physx::PxController* m_playerController;
	physx::PxControllerManager* m_characterManager;
	//PhysicsScene* m_PhysicsScene; 
	ControllerHitReport* m_hitReport;
	physx::PxMaterial* m_playerMaterial;
	physx::PxExtendedVec3 startPosition;
	physx::PxCapsuleGeometry capsule;
	physx::PxRigidDynamic* m_playerCapsule;
};

