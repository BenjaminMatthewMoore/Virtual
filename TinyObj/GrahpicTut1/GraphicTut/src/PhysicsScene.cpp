#include <stdlib.h>  
#include "PhysicsScene.h"
using namespace physx;

class MemoryAllocator : public PxAllocatorCallback
{
public:
	virtual ~MemoryAllocator() {}
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}
	virtual void deallocate(void* ptr)
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

void PhysicsScene::SetUpPhysx()
{
	PxAllocatorCallback *myCallback = new MemoryAllocator();
	
}