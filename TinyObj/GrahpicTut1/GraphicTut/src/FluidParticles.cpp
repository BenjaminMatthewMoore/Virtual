#include	"FluidParticles.h"
#include	<PxPhysicsAPI.h>
#include	"particles\PxParticleSystem.h"
#include	"particles\PxParticleFluid.h"
#include	"particles\PxParticleBase.h"
#include	"particles\PxParticleCreationData.h"
#include	"ParticleEmitter.h"
#include	"ParticleFluidEmitter.h"
#include	"PhysicsScene.h"
using namespace physx;

FluidParticles::FluidParticles()
{

}

FluidParticles::~FluidParticles()
{
}

void FluidParticles::StartUp(PhysicsScene* physicsScene)
{
	PxU32 m_maxParticles;
	m_maxParticles = 4000;
	perPartilceRestOffset = false;
	particleSystem = physicsScene->m_physics->createParticleSystem(m_maxParticles, perPartilceRestOffset);
	particleSystem->setDamping(0.1);
	particleSystem->setParticleMass(.1);
	particleSystem->setRestitution(0);
	particleSystem->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
	physicsScene->m_physicsScene->addActor(*particleSystem);
	m_particleEmitter = new ParticleEmitter(m_maxParticles, PxVec3(10, 10, 10), particleSystem, .01);
	m_particleEmitter->setStartVelocityRange(2.0f, 0, -2.0f, 2.0f, 0.0f, 2.0f);

}

void FluidParticles::Update(float deltaTime)
{
	m_particleEmitter->update(deltaTime);
	m_particleEmitter->renderParticles();
}