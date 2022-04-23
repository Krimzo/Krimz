#include "Physics/Physics.h"

#include "Utility/Utility.h"
#include "Data/Entities.h"
#include "Logging/Logging.h"


// Setup
void Engine::Physics::Init() {
	// Foundation creation
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCB, errorCB);
	if (!foundation) {
		kl::console::show();
		std::cout << "PhysX: Failed to create foundation!";
		std::cin.get();
		exit(69);
	}

	// Physics creation
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
	if (!physics) {
		kl::console::show();
		std::cout << "PhysX: Failed to create physics!";
		std::cin.get();
		exit(69);
	}

	// Cooking
	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physics->getTolerancesScale()));
	if (!cooking) {
		kl::console::show();
		std::cout << "PhysX: Failed to create cooking!";
		std::cin.get();
		exit(69);
	}
}
void Engine::Physics::Uninit() {
	cooking->release();
	physics->release();
	foundation->release();
}

// Scene
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
			ent.collider.delShape();
			ent.collider.delActor();
			ent.collider.delMaterial();
		}
		scene->release();
		scene = nullptr;
	}
}

// Frame update
void Engine::Physics::Update() {
	// Setting sim data
	for (auto& ent : Engine::entities) {
		// Actor creation
		ent.collider.newActor(ent.dynamic);

		// Data setting
		ent.collider.setGravity(ent.gravity);
		ent.collider.setFriction(ent.friction);
		ent.collider.setMass(ent.mass);
		ent.collider.setWorldRotation(ent.rotation);
		ent.collider.setWorldPosition(ent.position);
		ent.collider.setVelocity(ent.velocity);
		ent.collider.setAngular(ent.angular);

		// Shape creation
		switch (ent.collider.shape) {
		case Engine::Collider::Shape::Box:
			ent.collider.newShape(ent.scale * ent.collider.scale);
			break;

		case Engine::Collider::Shape::Sphere:
			ent.collider.newShape(ent.collider.scale.y);
			break;

		case Engine::Collider::Shape::Capsule:
			ent.collider.newShape(kl::float2(ent.collider.scale.y, ent.collider.scale.x));
			break;

		case Engine::Collider::Shape::Mesh:
			ent.collider.newShape(ent.mesh, ent.scale * ent.collider.scale);
			break;
		}

		// Adding actor to scene
		scene->addActor(*ent.collider.actor);
	}

	// Simulation
	scene->simulate(Engine::Time::delta);
	scene->fetchResults(true);

	// Reading sim data
	for (auto& ent : Engine::entities) {
		if (ent.dynamic) {
			ent.position = ent.collider.getWorldPosition() - ent.collider.position;
			ent.rotation = ent.collider.getWorldRotation() - ent.collider.rotation;
			ent.velocity = ent.collider.getVelocity();
			ent.angular = ent.collider.getAngular();
		}
		scene->removeActor(*ent.collider.actor);
	}
}
