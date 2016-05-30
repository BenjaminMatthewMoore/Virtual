#pragma once

namespace physx {
	class PxActor;
}


class Shapes
{
public:
	Shapes();
	~Shapes();
	void AddShape(physx::PxActor* actor);
private:

};