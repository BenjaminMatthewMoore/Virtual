#pragma once
#include "PxBroadPhase.h"

class PlayerController
{
public:
	PlayerController();
	~PlayerController();

private:
	bool onGround;
	float movementSpeed;
	float roatationSpeed;

};

