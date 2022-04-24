#include "Physics/Collider.h"

#include "Physics/Physics.h"


Engine::Collider::Collider() {}
Engine::Collider::Collider(const Collider& coll) {
	memcpy(this, &coll, sizeof(Engine::Collider));
	((Engine::Collider*)&coll)->canDelete = false;
	canDelete = true;
}
Engine::Collider::~Collider() {
	if (canDelete) {
		delShape();
		delActor();
		delMaterial();
	}
}

// Transform
physx::PxTransform Engine::Collider::getTransform(const kl::float3& entPos, const kl::float3 entRot) const {
	physx::PxTransform tempTra;
	const kl::float3 sumPos = entPos + position;
	const kl::float4 sumRot = kl::math::eulToQuat(entRot + rotation);
	tempTra.p = *(physx::PxVec3*)&sumPos;
	tempTra.q = *(physx::PxQuat*)&sumRot;
	return tempTra;
}

// Actor
void Engine::Collider::delActor() {
	if (actor) {
		actor->release();
		actor = nullptr;
	}
}
void Engine::Collider::newActor(bool dynamic, const kl::float3& entPos, const kl::float3 entRot) {
	delActor();
	if (dynamic) {
		actor = Engine::Physics::physics->createRigidDynamic(getTransform(entPos, entRot));
	}
	else {
		actor = Engine::Physics::physics->createRigidStatic(getTransform(entPos, entRot));
	}
	this->dynamic = dynamic;
}

// Material
void Engine::Collider::setFriction(float val) {
	if (material) {
		material->setStaticFriction(val);
		material->setDynamicFriction(val);
	}
	else {
		material = Engine::Physics::physics->createMaterial(val, val, 0.5f);
	}
}
void Engine::Collider::delMaterial() {
	if (material) {
		material->release();
		material = nullptr;
	}
}

// Data setting
void Engine::Collider::setGravity(bool enabled) {
	if (actor) {
		actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enabled);
	}
}
void Engine::Collider::setMass(float val) {
	if (actor && dynamic) {
		((physx::PxRigidDynamic*)actor)->setMass(val);
	}
}
void Engine::Collider::setWorldRotation(const kl::float3& entRot) {
	if (actor) {
		const kl::float4 sumRot = kl::math::eulToQuat(entRot + rotation);
		physx::PxTransform oldTransform = actor->getGlobalPose();
		oldTransform.q = *(physx::PxQuat*)&sumRot;
		actor->setGlobalPose(oldTransform);
	}
}
void Engine::Collider::setWorldPosition(const kl::float3& entPos) {
	if (actor) {
		const kl::float3 sumPos = entPos + position;
		physx::PxTransform oldTransform = actor->getGlobalPose();
		oldTransform.p = *(physx::PxVec3*)&sumPos;
		actor->setGlobalPose(oldTransform);
	}
}
void Engine::Collider::setVelocity(const kl::float3& vel) {
	if (actor && dynamic) {
		((physx::PxRigidDynamic*)actor)->setLinearVelocity(*(physx::PxVec3*)&vel);
	}
}
void Engine::Collider::setAngular(const kl::float3& ang) {
	if (actor && dynamic) {
		((physx::PxRigidDynamic*)actor)->setAngularVelocity(*(physx::PxVec3*)&ang);
	}
}

// Shape
void Engine::Collider::delShape() {
	if (shapeP) {
		if (actor && actor->getNbShapes()) {
			actor->detachShape(*shapeP);
		}
		shapeP->release();
		shapeP = nullptr;
	}
}
void Engine::Collider::newShape(const physx::PxGeometry& geo) {
	delShape();
	shapeP = Engine::Physics::physics->createShape(geo, *material);
	if (actor) {
		actor->attachShape(*shapeP);
	}
}
void Engine::Collider::newShape(const kl::float3& sca) {
	newShape(physx::PxBoxGeometry(*(physx::PxVec3*)&sca));
}
void Engine::Collider::newShape(float radius) {
	newShape(physx::PxSphereGeometry(radius));
}
void Engine::Collider::newShape(const kl::float2& heiRad) {
	newShape(physx::PxCapsuleGeometry(heiRad.x, heiRad.y));
}
void Engine::Collider::newShape(Engine::Mesh* mesh, const kl::float3& sca) {
	newShape(physx::PxTriangleMeshGeometry(mesh->cooked, *(physx::PxVec3*)&sca));
}

// Data getting
kl::float3 Engine::Collider::getWorldRotation() const {
	if (actor) {
		physx::PxTransform transform = actor->getGlobalPose();
		return kl::math::quatToEul(*(kl::float4*)&transform.q);
	}
	return {};
}
kl::float3 Engine::Collider::getWorldPosition() const {
	if (actor) {
		physx::PxTransform transform = actor->getGlobalPose();
		return *(kl::float3*)&transform.p;
	}
	return {};
}

kl::float3 Engine::Collider::getVelocity() const {
	if (actor && dynamic) {
		const physx::PxVec3 linVel = ((physx::PxRigidDynamic*)actor)->getLinearVelocity();
		return *(kl::float3*)&linVel;
	}
	return {};
}
kl::float3 Engine::Collider::getAngular() const {
	if (actor && dynamic) {
		const physx::PxVec3 angVel = ((physx::PxRigidDynamic*)actor)->getAngularVelocity();
		return *(kl::float3*)&angVel;
	}
	return {};
}

// Type name
String Engine::Collider::getName(const Collider::Shape& shape) {
	switch (shape) {
	case Engine::Collider::Shape::Box:
		return "Box";
	case Engine::Collider::Shape::Sphere:
		return "Sphere";
	case Engine::Collider::Shape::Capsule:
		return "Capsule";
	case Engine::Collider::Shape::Mesh:
		return "Mesh";
	default:
		return "None";
	}
}
