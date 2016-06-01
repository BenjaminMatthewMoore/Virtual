#include "PlayerController.h"
#include "ControllerHitReport.h"
#include <PxPhysicsAPI.h>
#include "PhysicsScene.h"
using namespace physx;

PlayerController::PlayerController(PxScene& physicScene)
{
	m_hitReport = new ControllerHitReport();
	m_characterManager = PxCreateControllerManager(physicScene);
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = m_playerMaterial;
	desc.reportCallback = m_hitReport;
	desc.density = 10;
	m_playerController = m_characterManager->createController(desc);
	m_playerController->setPosition(startPosition);
	m_velocity = 0;
	m_rotation = 0;
	m_gravity = -0.5f;
	m_hitReport->clearPlayerContactNormal();
	capsule.radius = 10;
	capsule.halfHeight = 5;
	//m_playerCapsule = PxCreateDynamic(physicScene., PxTransform(0,0,0), capsule, physicScene.)
}

PlayerController::~PlayerController()
{
}