#include "Shapes.h"
#include "Gizmos.h"
#include <PxPhysicsAPI.h>
using namespace physx;

Shapes::Shapes()
{
	
}

Shapes::~Shapes()
{
}


void Shapes::AddShape(PxActor* actor)
{
	if (actor->isRigidActor()) {
		auto rigidActor = (PxRigidActor*)actor;

		//PxShape** 

		//rigidActor->getShapes();

		PxShape* pShape;

	}
}