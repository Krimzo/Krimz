#include "Physics/Physics.h"

#pragma comment(lib, "PhysX_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXCommon_static_64.lib")
#pragma comment(lib, "PhysXCooking_static_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXFoundation_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXVehicle_static_64.lib")


Krimz::Physics::Physics()
{
	PxDefaultErrorCallback m_ErrorCallback;
	PxDefaultAllocator m_AllocatorCallback;

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_AllocatorCallback, m_ErrorCallback);
	kl::console::error(!m_Foundation, "Failed to create physics foundation");

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale());
	kl::console::error(!m_Physics, "Failed to create physics");

	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxCookingParams(m_Physics->getTolerancesScale()));
	kl::console::error(!m_Cooking, "Failed to create physics cooking");

	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity.y = -9.81f;
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);
}

Krimz::Physics::~Physics()
{
	m_Scene->release();
	m_Dispatcher->release();
	m_Cooking->release();
	m_Physics->release();
	m_Foundation->release();
}

PxPhysics* Krimz::Physics::physics()
{
	return m_Physics;
}

void Krimz::Physics::add(kl::ref<Physical> physical)
{
	m_Scene->addActor(physical->actor());
}

void Krimz::Physics::remove(kl::ref<Physical> physical)
{
	m_Scene->removeActor(physical->actor());
}

void Krimz::Physics::update(float deltaTime)
{
	m_Scene->simulate(deltaTime);
	m_Scene->fetchResults(true);
}
