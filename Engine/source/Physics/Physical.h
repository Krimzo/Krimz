#pragma once

#include "KrimzLib.h"
#include "GUI/GUIRenderable.h"
#include "Transform/Transform.h"
#include "Physics/Collider.h"


namespace Krimz {
	class Physical {
		PxRigidActor* m_Actor = nullptr;

	public:
		static PxQuat ConvertQuat(const kl::float3& euler);
		static kl::float3 ConvertQuat(const PxQuat& quat);

		static PxTransform ConvertTransform(const Transform& transform);
		static Transform ConvertTransform(const PxTransform& transform);

		Physical(PxPhysics* physics, bool dynamic);
		Physical(const Physical&) = delete;
		void operator=(const Physical&) = delete;
		~Physical();

		PxActor& actor();

		PxRigidDynamic* dynamic() const;
		void dynamic(PxPhysics* physics, bool dynamic);

		// Geometry
		kl::float3 rotation() const;
		void rotation(const kl::float3& rotation);

		kl::float3 position() const;
		void position(const kl::float3& position);

		Transform transform() const;
		void transform(const Transform& transform);

		// Movement
		kl::float3 velocity() const;
		void velocity(const kl::float3& velocity);

		kl::float3 angular() const;
		void angular(const kl::float3& angular);

		bool gravity() const;
		void gravity(bool gravity);

		float mass() const;
		void mass(float mass);

		// Shape
		void attach(kl::ref<Collider> collider);
		void detach(kl::ref<Collider> collider);
	};
}
