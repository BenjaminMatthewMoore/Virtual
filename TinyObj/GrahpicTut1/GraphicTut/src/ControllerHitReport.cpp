#include "ControllerHitReport.h"
ControllerHitReport::ControllerHitReport()
{
}

ControllerHitReport::~ControllerHitReport()
{
}

void ControllerHitReport::onShapeHit(const PxControllerShapeHit &hit)
{
	PxRigidActor* actor = hit.actor;
	m_playerContactNormal = hit.worldNormal;

	PxRigidDynamic* myActor = <PxRigidDynamic>();

}