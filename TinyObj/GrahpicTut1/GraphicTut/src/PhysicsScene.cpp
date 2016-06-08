#include <stdlib.h>  
#include "PhysicsScene.h"
#include "Gizmos.h"

#include <extensions\PxDefaultAllocator.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <PxPhysicsAPI.h>
#include "ControllerHitReport.h"
#include "PlayerController.h"
#include "TerrainGen.h"
#include "Camera.h"
#include "Window.h"
#include "FluidParticles.h"

using namespace physx;

class MemoryAllocator : public PxAllocatorCallback
{
public:
	virtual ~MemoryAllocator() {}
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}
	virtual void deallocate(void* ptr) override
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

void PhysicsScene::SetUpPhysx(float TerrainSize)
{
	mDefaultAllocatorCallback = new MemoryAllocator();
	mDefaultErrorCallback = new PxDefaultErrorCallback();
	m_physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *mDefaultAllocatorCallback, *mDefaultErrorCallback);
	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_physicsFoundation, PxTolerancesScale());
	PxInitExtensions(*m_physics);
	m_physicsMaterial = m_physics->createMaterial(0.5f, 0.5f, 0.5f);
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	m_physicsScene = m_physics->createScene(sceneDesc);
	m_playerController = new PlayerController(this);
	PxBoxGeometry box(5, 5, 5);
	PxTransform transform(PxVec3(20, 100, 50));
	PxRigidDynamic* boxActor = PxCreateDynamic(*m_physics, transform, box, *m_physicsMaterial, 10.f);
	m_physicsScene->addActor(*boxActor);
	//copied
	PxTransform poseOriginLeft = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(0,PxVec3(0.f, 0.0f, 0.0f)));
	PxRigidStatic* planeLeft = PxCreateStatic(*m_physics, poseOriginLeft, PxPlaneGeometry(),*m_physicsMaterial);
	m_physicsScene->addActor(*planeLeft);

	PxTransform poseFarsideRight = PxTransform(PxVec3(TerrainSize, 0.0f, 0.0f), PxQuat(PxHalfPi*2, PxVec3(0.0f, 1.0f, 0.0f)));
	PxRigidStatic* planeRight = PxCreateStatic(*m_physics, poseFarsideRight, PxPlaneGeometry(), *m_physicsMaterial);
	m_physicsScene->addActor(*planeRight);

	PxTransform poseOriginFront = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi*3.0f, PxVec3(0.0f, 1.0f, 0.0f)));
	PxRigidStatic* planeFarSide = PxCreateStatic(*m_physics, poseOriginFront, PxPlaneGeometry(), *m_physicsMaterial);
	m_physicsScene->addActor(*planeFarSide);

	PxTransform poseNearSide = PxTransform(PxVec3(0.0f, 0.0f, TerrainSize), PxQuat(PxHalfPi*1.0f, PxVec3(0.0f, 1.0f, 0.0f)));
	PxRigidStatic* planeNearSide = PxCreateStatic(*m_physics, poseNearSide, PxPlaneGeometry(), *m_physicsMaterial);
	m_physicsScene->addActor(*planeNearSide);

	PxTransform baseline = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi*1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* planeBase = PxCreateStatic(*m_physics, baseline, PxPlaneGeometry(), *m_physicsMaterial);
	m_physicsScene->addActor(*planeBase);

	fluidSystem = new FluidParticles;
	fluidSystem->StartUp(this);

}

void PhysicsScene::DrawScene()
{

	PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
	PxU32 actor_count = m_physicsScene->getNbActors(desiredTypes);
	PxActor** actor_list = new PxActor*[actor_count];
	m_physicsScene->getActors(desiredTypes, actor_list, actor_count);

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

void PhysicsScene::Update(float a_deltaTime, Window* window)
{
	if (a_deltaTime <= 0)
	{
		return;
	}
	m_physicsScene->simulate(a_deltaTime);
	while (m_physicsScene->fetchResults() == false)
	{
	}
	m_playerController->Update(a_deltaTime, window);
	fluidSystem->Update(a_deltaTime);
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
			Gizmos::addCapsule(actor_position, geo.halfHeight * 4, geo.radius, 16, 16, geo_color, &rot);
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

	void PhysicsScene::CreateHeightMap(TerrainGen terrain)
	{
		
		PxTransform transform(PxVec3(0, 0, 0));
		
		PxHeightFieldDesc hfDesc;
		hfDesc.format = PxHeightFieldFormat::eS16_TM;
		hfDesc.nbColumns = terrain.m_dimensions;
		hfDesc.nbRows = terrain.m_dimensions;
		int *perlinAdjusted = new int[terrain.m_dimensions*terrain.m_dimensions];
		for (int i = 0; i < terrain.m_dimensions*terrain.m_dimensions; i++)
		{
			perlinAdjusted[i] = static_cast<int>((terrain.m_perlinData[i]) * 5);
		}
		hfDesc.samples.data = perlinAdjusted;
		hfDesc.samples.stride = sizeof(PxHeightFieldSample);
		hfDesc.thickness = -100.0f;

		PxHeightField *aHeightField = m_physics->createHeightField(hfDesc);
		PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), 1, 1, 1);
		PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f));
		//PxShape* heightmap = m_pXActor->createShape(hfGeom, *m_physicsMaterial, pose);

		m_pXActor = PxCreateStatic(*m_physics, transform, hfGeom,*m_physicsMaterial);
		m_physicsScene->addActor(*m_pXActor);
	}

	void PhysicsScene::setUpVisualDebugger()
	{
		if (m_physics->getPvdConnectionManager() == NULL)
			return;
		const char* pvd_host_ip = "127.0.0.1";
		int port = 5425;
		unsigned int timeout = 100;
		PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
		auto theConnection = PxVisualDebuggerExt::createConnection(m_physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
	}

	void PhysicsScene::ShootCubes(Camera* camera)
	{
		//Transform
		vec3 cam_pos = glm::vec3 (camera->worldTransform[3]);
		vec3 box_vel = glm::vec3(-camera->worldTransform[2] * 20.f);
		PxTransform box_transform(PxVec3(cam_pos.x, cam_pos.y, cam_pos.z));
		//Geometry
		PxSphereGeometry sphere(5.f);
		//Density
		float density = 10;
		PxRigidDynamic* new_actor = PxCreateDynamic(*m_physics,
			box_transform,
			sphere,
			*m_physicsMaterial,
			density);
		glm::vec3 direction(-camera->worldTransform[2]);
		physx::PxVec3 velocity = physx::PxVec3(direction.x, direction.y, direction.z) * 10;
		new_actor->setLinearVelocity(velocity, true);
		m_physicsScene->addActor(*new_actor);
	}