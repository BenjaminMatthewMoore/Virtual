#pragma once
#include <PxPhysicsAPI.h>
using namespace physx;
class ControllerHitReport : public PxUserControllerHitReport
{
public:
	~ControllerHitReport();

	///overload
	virtual void onShapeHit(const PxControllerShapeHit &hit)override;
	virtual void onControllerHit(const PxControllersHit &hit) override{};
	virtual void onObstacleHit(const PxControllerObstacleHit &hit)override {};
	ControllerHitReport() : PxUserControllerHitReport(){};
	PxVec3 getPlayerContactNormal() { return m_playerContactNormal; };
	void clearPlayerContactNormal() { m_playerContactNormal = PxVec3(0, 0, 0); };
	PxVec3 m_playerContactNormal;
};

