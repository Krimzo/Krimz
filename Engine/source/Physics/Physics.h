#pragma once

#include "KrimzLib.h"
#include "Physics/Physical.h"


namespace Krimz
{
	class Physics
	{
		PxFoundation* m_Foundation = nullptr;
		PxPhysics* m_Physics = nullptr;
		PxCooking* m_Cooking = nullptr;
		PxDefaultCpuDispatcher* m_Dispatcher = nullptr;
		PxScene* m_Scene = nullptr;

	public:
		Physics();
		Physics(const Physics&) = delete;
		void operator=(const Physics&) = delete;
		~Physics();

		PxPhysics* physics();

		void add(kl::ref<Physical> physical);
		void remove(kl::ref<Physical> physical);

		void update(float deltaTime);
	};
}
