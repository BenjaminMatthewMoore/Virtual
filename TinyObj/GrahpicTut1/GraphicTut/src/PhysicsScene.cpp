#include <stdlib.h>  
#include "PhysicsScene.h"
#include "Gizmos.h"

#include <extensions\PxDefaultAllocator.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <PxPhysicsAPI.h>
#include "ControllerHitReport.h"
#include "PlayerController.h"

using namespace physx;

class MemoryAllocator : public PxAllocatorCallback
{
public:
	virtual ~MemoryAllocator() {}
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}
	virtual void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}
};
PhysicsScene::PhysicsScene()
{
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::SetUpPhysx()
{
	PxAllocatorCallback *callback = new MemoryAllocator();
	m_physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *callback, mDefaultErrorCallback);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_physicsFoundation, PxTolerancesScale());
	PxInitExtensions(*m_physics);
	m_physicsMaterial = m_physics->createMaterial(0.5f, 0.5f, 0.5f);
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	m_physicsScene = m_physics->createScene(sceneDesc);

}

void PhysicsScene::DrawScene()
{

	PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
	PxU32 actor_count = m_pScene->getNbActors(desiredTypes);
	PxActor** actor_list = new PxActor*[actor_count];
	m_pScene->getActors(desiredTypes, actor_list, actor_count);

	glm::vec4 geo_color(1, 0, 0, 1);
	for (int actor_index = 0;
	actor_index < (int)actor_count;
		++actor_index)
	{
		PxActor* curr_actor = actor_list[actor_index];
		if (curr_actor->isRigidActor())
		{
			PxRigidActor* rigid_actor = (PxRigidActor*)curr_actor;
			PxU32 shape_count = rigid_actor->getNbShapes();
			PxShape** shapes = new PxShape*[shape_count];
			rigid_actor->getShapes(shapes, shape_count);

			for (int shape_index = 0;
			shape_index < (int)shape_count;
				++shape_index)
			{
				PxShape* curr_shape = shapes[shape_index];
				AddWidget(curr_shape, rigid_actor, geo_color);
			}

			delete[]shapes;
		}
	}
}

void PhysicsScene::Update(float a_deltaTime)
{
	if (a_deltaTime <= 0)
	{
		return;
	}
	m_physicsScene->simulate(a_deltaTime);
	while (m_physicsScene->fetchResults() == false)
	{
	}
}


	void PhysicsScene::AddWidget(PxShape* shape, PxRigidActor* actor, glm::vec4 geo_color)
	{
		PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
		glm::vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
		glm::quat actor_rotation(full_transform.q.w,
			full_transform.q.x,
			full_transform.q.y,
			full_transform.q.z);
		glm::mat4 rot(actor_rotation);

		glm::mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

		PxGeometryType::Enum geo_type = shape->getGeometryType();

		switch (geo_type)
		{
		case (PxGeometryType::eBOX) :
		{
			PxBoxGeometry geo;
			shape->getBoxGeometry(geo);
			glm::vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
			Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
		} break;
		case (PxGeometryType::eCAPSULE) :
		{
			PxCapsuleGeometry geo;
			shape->getCapsuleGeometry(geo);
			Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
		} break;
		case (PxGeometryType::eSPHERE) :
		{
			PxSphereGeometry geo;
			shape->getSphereGeometry(geo);
			Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
		} break;
		case (PxGeometryType::ePLANE) :
		{

		} break;
		}
	}


