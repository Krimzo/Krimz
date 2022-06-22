#pragma once

#include <PxPhysicsAPI.h>


namespace Engine {
	namespace Physics {
		inline physx::PxDefaultErrorCallback errorCallback;
		inline physx::PxDefaultAllocator allocatorCallback;
		inline physx::PxFoundation* foundation = nullptr;
		inline physx::PxPhysics* physics = nullptr;
		inline physx::PxCooking* cooking = nullptr;
		inline physx::PxCpuDispatcher* dispatcher = nullptr;
		inline physx::PxScene* scene = nullptr;

		void Init();
		void Uninit();

		void CreateScene();
		void DestroyScene();

		void Update();
	}
}
