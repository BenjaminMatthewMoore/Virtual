#include "PlayerController.h"
#include "ControllerHitReport.h"

#include "PhysicsScene.h"
#include "Gizmos.h"
#include "Window.h"
#include "GLFW\glfw3.h"
using namespace physx;

PlayerController::PlayerController(PhysicsScene* physicScene)
{
	startPosition = PxExtendedVec3(20, 20, 20);
	m_hitReport = new ControllerHitReport();
	m_characterManager = PxCreateControllerManager(*physicScene->m_physicsScene);
	PxCapsuleControllerDesc desc;
	desc.height = 2.0f;
	desc.radius = 1.0f;
	//desc.position.set(0, 0, 0);z
	desc.material = physicScene->m_physicsMaterial;
	desc.reportCallback = m_hitReport;
	desc.density = 10;
	m_playerController = m_characterManager->createController(desc);
	m_playerController->setPosition(startPosition);
	physicScene->m_physicsScene->addActor(*m_playerController->getActor());
	m_velocity = 10;
	m_rotation = 0;
	m_gravity = -9.8f;
	m_hitReport->clearPlayerContactNormal();
	//capsule.radius = 5;
	//capsule.halfHeight = 30;
	//m_playerController = PxCreateDynamic(*physicScene->m_physics, PxTransform(60, 60, 60), capsule, *physicScene->m_physicsMaterial, (PxReal)desc.density);
	//physicScene->m_physicsScene->addActor(*m_playerCapsule);
	bool onGround = true;
	movementSpeed = 20.0f;
	roatationSpeed = 1.0f;
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update(float deltaTime, Window* activeWindow)
{
	if (m_hitReport->getPlayerContactNormal().y > 0.1f)
	{
		m_velocity = -0.01f;
		onGround = true;
	}
	else 
	{
		m_velocity += m_gravity*deltaTime;
		onGround = false;
	}

	m_hitReport->clearPlayerContactNormal();
	
	PxVec3 velocity(0, m_velocity, 0);
	if (glfwGetKey(activeWindow->m_window, GLFW_KEY_J) == GLFW_PRESS)
	{
		velocity.x = -movementSpeed*deltaTime;
	}
	if (glfwGetKey(activeWindow->m_window, GLFW_KEY_L) == GLFW_PRESS)
	{
		velocity.x = +movementSpeed*deltaTime;
	}

	if (glfwGetKey(activeWindow->m_window, GLFW_KEY_I) == GLFW_PRESS)
	{
		velocity.z = -movementSpeed*deltaTime;
	}
	if (glfwGetKey(activeWindow->m_window, GLFW_KEY_K) == GLFW_PRESS)
	{
		velocity.z = +movementSpeed*deltaTime;
	}
	const PxVec3 up = PxVec3(0, 1, 0);
	float minDistance = 0.001f;
	PxControllerFilters filter;

	
	PxQuat rotation(m_rotation, PxVec3(0, 1, 0));
	m_playerController->move(rotation.rotate(velocity), minDistance, deltaTime, filter);
}