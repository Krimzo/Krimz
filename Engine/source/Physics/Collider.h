#pragma once

#include "KrimzLib.h"
#include "PxPhysicsAPI.h"
#include "Mesh/Mesh.h"

using namespace physx;


namespace Krimz
{
	class Collider
	{
	private:
		PxShape* m_Shape = nullptr;
		PxMaterial* m_Material = nullptr;

	public:
		static PxVec3 ConvertVector(const kl::float3& vector);
		static kl::float3 ConvertVector(const PxVec3& vector);

		Collider(PxPhysics* physics, const PxGeometry& geometry);
		Collider(PxPhysics* physics, const kl::float3& scale);
		Collider(PxPhysics* physics, float radius);
		Collider(PxPhysics* physics, float height, float radius);
		Collider(PxPhysics* physics, kl::ref<Mesh> mesh, const kl::float3& scale);
		Collider(const Collider&) = delete;
		void operator=(const Collider&) = delete;
		~Collider();

		// Material
		float staticFriction() const;
		void staticFriction(float friction);

		float dynamicFriction() const;
		void dynamicFriction(float friction);

		float restitution() const;
		void restitution(float restitution);

		// Shape
		PxShape& shape() const;
		std::string type() const;
	};
}
