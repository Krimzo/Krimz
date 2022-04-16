#include "Physics/Physics.h"

#include "Utility/Time.h"
#include "Data/Entities.h"


// Setup
void Engine::Physics::Init()
{
	// Foundation creation
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCB, errorCB);
	if (!foundation)
	{
		kl::console::show();
		std::cout << "PhysX: Failed to create foundation!";
		std::cin.get();
		exit(69);
	}

	// Physics creation
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
	if (!physics)
	{
		kl::console::show();
		std::cout << "PhysX: Failed to create physics!";
		std::cin.get();
		exit(69);
	}

	// Cooking
	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physics->getTolerancesScale()));
	if (!cooking)
	{
		kl::console::show();
		std::cout << "PhysX: Failed to create cooking!";
		std::cin.get();
		exit(69);
	}
}
void Engine::Physics::Uninit()
{
	cooking->release();
	physics->release();
	foundation->release();
}

// Scene
void Engine::Physics::CreateScene()
{
	static physx::PxDefaultCpuDispatcher* workerThreads = physx::PxDefaultCpuDispatcherCreate(2);
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity.y = -9.81f;
	sceneDesc.cpuDispatcher = workerThreads;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);
}
void Engine::Physics::DestroyScene()
{
	if (scene)
		scene->release();
	scene = nullptr;
}

// Frame update
void Engine::Physics::Update()
{
	// Material setup
	physx::PxMaterial* testMaterial = physics->createMaterial(0.5f, 0.5f, 0.5f);

	// Setting sim data
	for (int i = 0; i < Engine::entities.size(); i++)
	{
		// Transform calculation
		const kl::float4 quat = kl::math::eulToQuat(Engine::entities[i]->rotation);
		physx::PxTransform entTran(
			physx::PxVec3(Engine::entities[i]->position.x, Engine::entities[i]->position.y, Engine::entities[i]->position.z),
			physx::PxQuat(quat.x, quat.y, quat.z, quat.w));

		// Physics setup
		if (Engine::entities[i]->dynamic)
		{
			// Actor creation
			Engine::entities[i]->actor = physics->createRigidDynamic(entTran);
			((physx::PxRigidDynamic*)Engine::entities[i]->actor)->setLinearVelocity(
				physx::PxVec3(Engine::entities[i]->velocity.x, Engine::entities[i]->velocity.y, Engine::entities[i]->velocity.z));
			((physx::PxRigidDynamic*)Engine::entities[i]->actor)->setAngularVelocity(
				physx::PxVec3(Engine::entities[i]->angular.x, Engine::entities[i]->angular.y, Engine::entities[i]->angular.z));

			// Collider creation
			if (Engine::entities[i]->collisions)
			{
				physx::PxShape* boxShape = physics->createShape(
					physx::PxBoxGeometry(Engine::entities[i]->scale.x,
						Engine::entities[i]->scale.y, Engine::entities[i]->scale.z), *testMaterial);
				Engine::entities[i]->actor->attachShape(*boxShape);
				boxShape->release();
			}
		}
		else
		{
			// Actor creation
			Engine::entities[i]->actor = physics->createRigidStatic(entTran);

			// Collider creation
			if (Engine::entities[i]->collisions)
			{
				physx::PxShape* boxShape = physics->createShape(
					physx::PxBoxGeometry(Engine::entities[i]->scale.x,
						Engine::entities[i]->scale.y, Engine::entities[i]->scale.z), *testMaterial);
				Engine::entities[i]->actor->attachShape(*boxShape);
				boxShape->release();
			}
		}

		// Adding actor to scene
		scene->addActor(*Engine::entities[i]->actor);
	}

	// Simulation
	scene->simulate(Engine::Time::delta);
	scene->fetchResults(true);

	// Reading sim data
	for (int i = 0; i < Engine::entities.size(); i++)
	{
		if (Engine::entities[i]->dynamic)
		{
			physx::PxTransform tran = Engine::entities[i]->actor->getGlobalPose();
			physx::PxVec3 vel = ((physx::PxRigidDynamic*)Engine::entities[i]->actor)->getLinearVelocity();
			physx::PxVec3 ang = ((physx::PxRigidDynamic*)Engine::entities[i]->actor)->getAngularVelocity();
			Engine::entities[i]->position = kl::float3(tran.p.x, tran.p.y, tran.p.z);
			Engine::entities[i]->rotation = kl::math::quatToEul(*(kl::float4*)&tran.q);
			Engine::entities[i]->velocity = kl::float3(vel.x, vel.y, vel.z);
			Engine::entities[i]->angular = kl::float3(ang.x, ang.y, ang.z);
		}
	}

	// Cleanup
	for (int i = 0; i < Engine::entities.size(); i++)
	{
		scene->removeActor(*Engine::entities[i]->actor);
		Engine::entities[i]->actor->release();
	}
	testMaterial->release();
}
