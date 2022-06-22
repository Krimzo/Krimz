#include "Physics/Physics.h"
#include "Render/Render.h"
#include "Types/Entity.h"
#include "Logging/Logging.h"

#pragma comment(lib, "PhysX_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXCommon_static_64.lib")
#pragma comment(lib, "PhysXCooking_static_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXFoundation_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXVehicle_static_64.lib")


void Engine::Physics::Init() {
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);
	kl::console::error(!foundation, "Failed to create foundation");

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
	kl::console::error(!physics, "Failed to create physics");

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physics->getTolerancesScale()));
	kl::console::error(!cooking, "Failed to create cooking");
}
void Engine::Physics::Uninit() {
	cooking->release();
	cooking = nullptr;
	physics->release();
	physics = nullptr;
	foundation->release();
	foundation = nullptr;
}

void Engine::Physics::CreateScene() {
	static physx::PxDefaultCpuDispatcher* workerThreads = physx::PxDefaultCpuDispatcherCreate(2);
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity.y = -9.81f;
	sceneDesc.cpuDispatcher = workerThreads;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);
}
void Engine::Physics::DestroyScene() {
	if (scene) {
		for (auto& ent : Engine::entities) {
			ent->collider.delShape();
			ent->collider.delActor();
			ent->collider.delMaterial();
		}
		scene->release();
		scene = nullptr;
	}
}

void Engine::Physics::Update() {
	for (auto& entity : Engine::entities) {
		entity->collider.newActor(entity->dynamic);

		entity->collider.setGravity(entity->gravity);
		entity->collider.setFriction(entity->friction);
		entity->collider.setMass(entity->mass);
		entity->collider.setWorldRotation(entity->rotation);
		entity->collider.setWorldPosition(entity->position);
		entity->collider.setVelocity(entity->velocity);
		entity->collider.setAngular(entity->angular);

		switch (entity->collider.shape) {
		case Engine::Collider::Shape::Box:
			entity->collider.newShape(entity->scale * entity->collider.scale);
			break;

		case Engine::Collider::Shape::Sphere:
			entity->collider.newShape(entity->collider.scale.y);
			break;

		case Engine::Collider::Shape::Capsule:
			entity->collider.newShape(kl::float2(entity->collider.scale.y, entity->collider.scale.x));
			break;

		case Engine::Collider::Shape::Mesh:
			entity->collider.newShape(entity->mesh, entity->scale * entity->collider.scale);
			break;
		}

		scene->addActor(*entity->collider.actor);
	}

	scene->simulate(Engine::Time::delta);
	scene->fetchResults(true);

	for (auto& entity : Engine::entities) {
		if (entity->dynamic) {
			entity->position = entity->collider.getWorldPosition() - entity->collider.position;
			entity->rotation = entity->collider.getWorldRotation() - entity->collider.rotation;
			entity->velocity = entity->collider.getVelocity();
			entity->angular = entity->collider.getAngular();
		}
		scene->removeActor(*entity->collider.actor);
	}
}
