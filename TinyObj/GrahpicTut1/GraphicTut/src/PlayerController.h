#pragma once
#include "PxBroadPhase.h"
namespace physx {
	class PxController;
	class PxControllerManager;
}
class ControllerHitReport;
class PhysicsScene;

class PlayerController
{
public:

	PlayerController(physx::PxScene& physicScene);
	~PlayerController();

private:
	bool onGround;
	float movementSpeed;
	float roatationSpeed;
	float m_velocity;
	float m_rotation;
	float m_gravity;

	physx::PxController* m_playerController;
	physx::PxControllerManager* m_characterManager;
	PhysicsScene* m_PhysicsScene; 
	ControllerHitReport* m_hitReport;
	physx::PxMaterial* m_playerMaterial;
	physx::PxExtendedVec3 startPosition;
	physx::PxCapsuleGeometry capsule;
	physx::PxRigidDynamic* m_playerCapsule;
};

