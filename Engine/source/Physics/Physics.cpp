#include "Physics/Physics.h"

#ifdef _DEBUG
#pragma comment(lib, "debug/PhysX_static_64.lib")
#pragma comment(lib, "debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "debug/PhysXCommon_static_64.lib")
#pragma comment(lib, "debug/PhysXCooking_static_64.lib")
#pragma comment(lib, "debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "debug/PhysXFoundation_static_64.lib")
#pragma comment(lib, "debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "debug/PhysXVehicle_static_64.lib")
#else
#pragma comment(lib, "release/PhysX_static_64.lib")
#pragma comment(lib, "release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "release/PhysXCommon_static_64.lib")
#pragma comment(lib, "release/PhysXCooking_static_64.lib")
#pragma comment(lib, "release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "release/PhysXFoundation_static_64.lib")
#pragma comment(lib, "release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "release/PhysXVehicle_static_64.lib")
#endif


Krimz::Physics::Physics() {
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_Error);
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
	kl::console::error(!m_Scene, "Failed to create physics scene");
}

Krimz::Physics::~Physics() {
	m_Scene->release();
	m_Dispatcher->release();
	m_Cooking->release();
	m_Physics->release();
	m_Foundation->release();
}

PxPhysics* Krimz::Physics::physics() {
	return m_Physics;
}

void Krimz::Physics::bind(kl::ref<Scene> scene) {
	/* WIP */
}

void Krimz::Physics::add(kl::ref<Physical> physical) {
	m_Scene->addActor(physical->actor());
}

void Krimz::Physics::remove(kl::ref<Physical> physical) {
	m_Scene->removeActor(physical->actor());
}

void Krimz::Physics::update(float deltaTime) {
	m_Scene->simulate(deltaTime);
	m_Scene->fetchResults(true);
}
