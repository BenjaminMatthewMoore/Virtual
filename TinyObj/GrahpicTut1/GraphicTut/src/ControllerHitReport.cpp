#include "ControllerHitReport.h"

ControllerHitReport::~ControllerHitReport()
{
}

void ControllerHitReport::onShapeHit(const PxControllerShapeHit &hit)
{
	PxRigidActor* actor = hit.shape->getActor();
	m_playerContactNormal = hit.worldNormal;

	PxRigidDynamic* myActor = actor->is<PxRigidDynamic>();
	if (myActor)
	{
		//add forces
	}

}