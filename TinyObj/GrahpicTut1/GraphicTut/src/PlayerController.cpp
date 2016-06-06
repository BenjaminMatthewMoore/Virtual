#include "PlayerController.h"
#include "ControllerHitReport.h"
#include <PxPhysicsAPI.h>
#include "PhysicsScene.h"
#include "Gizmos.h"
using namespace physx;

PlayerController::PlayerController(PhysicsScene* physicScene)
{
	m_hitReport = new ControllerHitReport();
	m_characterManager = PxCreateControllerManager(*physicScene->m_physicsScene);
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = physicScene->m_physicsMaterial;
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
	m_playerCapsule = PxCreateDynamic(*physicScene->m_physics, PxTransform(0, 0, 0), capsule, *physicScene->m_physicsMaterial, (PxReal)desc.density);
	physicScene->m_physicsScene->addActor(*m_playerCapsule);

}

PlayerController::~PlayerController()
{
}