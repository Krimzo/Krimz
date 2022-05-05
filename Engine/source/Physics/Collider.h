#pragma once

#include "KrimzLib.h"
#include "Physics/Physics.h"
#include "Types/Mesh.h"


namespace Engine {
	class Collider {
	private:
		bool dynamic = false;

	public:
		// Shape types
		enum class Shape {
			None = 0,
			Box,
			Sphere,
			Capsule,
			Mesh
		};

		// Data
		kl::float3 scale = kl::float3(1.0f);
		kl::float3 rotation = {};
		kl::float3 position = {};
		Shape shape = Shape::None;
		physx::PxRigidActor* actor = nullptr;
		physx::PxShape* shapeP = nullptr;
		physx::PxMaterial* material = nullptr;

		Collider();
		Collider(const Collider& obj);
		void operator=(const Collider& obj);
		~Collider();

		// Transform
		physx::PxTransform getTransform(const kl::float3& entPos, const kl::float3 entRot) const;

		// Actor
		void delActor();
		void newActor(bool dynamic, const kl::float3& entPos = {}, const kl::float3 entRot = {});

		// Material
		void setFriction(float val);
		void delMaterial();

		// Data setting
		void setGravity(bool enabled);
		void setMass(float val);
		void setWorldRotation(const kl::float3& entRot);
		void setWorldPosition(const kl::float3& entPos);
		void setVelocity(const kl::float3& vel);
		void setAngular(const kl::float3& ang);

		// Shape
		void delShape();
		void newShape(const physx::PxGeometry& geo);
		void newShape(const kl::float3& sca);
		void newShape(float radius);
		void newShape(const kl::float2& heiRad);
		void newShape(const std::shared_ptr<Engine::Mesh>& mesh, const kl::float3& sca);

		// Data getting
		kl::float3 getWorldRotation() const;
		kl::float3 getWorldPosition() const;
		kl::float3 getVelocity() const;
		kl::float3 getAngular() const;

		// Type name
		static String getName(const Collider::Shape& shape);
	};
}
