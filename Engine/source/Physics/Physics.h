#pragma once

#include <PxPhysicsAPI.h>

#pragma comment(lib, "PhysX_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXCommon_static_64.lib")
#pragma comment(lib, "PhysXCooking_static_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXFoundation_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXVehicle_static_64.lib")


namespace Engine
{
	namespace Physics
	{
		inline physx::PxDefaultErrorCallback errorCB;
		inline physx::PxDefaultAllocator allocatorCB;
		inline physx::PxFoundation* foundation = nullptr;
		inline physx::PxPhysics* physics = nullptr;
		inline physx::PxCooking* cooking = nullptr;
		inline physx::PxCpuDispatcher* dispatcher = nullptr;
		inline physx::PxScene* scene = nullptr;

		// Setup
		void Init();
		void Uninit();

		// Scene
		void CreateScene();
		void DestroyScene();

		// Frame update
		void Update();
	}
}
