#pragma once
namespace physx {
	class PxParticleSystem;
}
class PhysicsScene;
class ParticleEmitter;
class PartilceFluidEmitter;


class FluidParticles
{
public:
	FluidParticles();
	~FluidParticles();

	void StartUp(PhysicsScene* physicsScene);
	physx::PxParticleSystem* particleSystem;
	ParticleEmitter* m_particleEmitter;
	void Update(float deltaTime);

private:
	bool perPartilceRestOffset;

};