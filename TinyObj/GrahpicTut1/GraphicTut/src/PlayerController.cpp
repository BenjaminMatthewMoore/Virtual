#include "PlayerController.h"
#include "ControllerHitReport.h"
#include <PxPhysicsAPI.h>
#include "PhysicsScene.h"
using namespace physx;

PlayerController::PlayerController(PxScene& physicScene)
{
	m_hitReport = new ControllerHitReport();
	m_characterManager = PxCreateControllerManager(physicScene);
}

PlayerController::~PlayerController()
{
}