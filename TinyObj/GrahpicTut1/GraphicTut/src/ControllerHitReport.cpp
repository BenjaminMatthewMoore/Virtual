#include "ControllerHitReport.h"

ControllerHitReport::~ControllerHitReport()
{
}

void ControllerHitReport::onShapeHit(const PxControllerShapeHit &hit)
{
	PxRigidActor* actor = hit.shape->getActor();
	m_playerContactNormal = hit.worldNormal;

	PxRigidDynamic* a_Actor = actor->is<PxRigidDynamic>();
	if (a_Actor)
	{	
	
		//add forces
	}

}