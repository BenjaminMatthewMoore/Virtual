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
	physx::PxController* m_playerController;
	physx::PxControllerManager* m_characterManager;
	PhysicsScene* m_PhysicsScene; 
	ControllerHitReport* m_hitReport;
};

