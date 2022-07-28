#include "Physics/Physical.h"


PxQuat Krimz::Physical::ConvertQuat(const kl::float3& euler)
{
	kl::float4 result = kl::to::quaternion(euler);
	return *(PxQuat*) &result;
}

kl::float3 Krimz::Physical::ConvertQuat(const PxQuat& quat)
{
	kl::float4 result = *(kl::float4*) &quat;
	return kl::to::euler(result);
}

PxTransform Krimz::Physical::ConvertTransform(const Transform& transform)
{
	PxTransform result = {};
	result.p = Collider::ConvertVector(transform.position);
	result.q = ConvertQuat(transform.rotation);
	return result;
}

Krimz::Transform Krimz::Physical::ConvertTransform(const PxTransform& transform)
{
	Transform result = {};
	result.position = Collider::ConvertVector(transform.p);
	result.rotation = ConvertQuat(transform.q);
	return result;
}

Krimz::Physical::Physical(PxPhysics* physics, bool dynamic)
{
	if (dynamic)
	{
		m_Actor = physics->createRigidDynamic({});
	}
	else
	{
		m_Actor = physics->createRigidStatic({});
	}
}

Krimz::Physical::~Physical()
{
	m_Actor->release();
}

PxActor& Krimz::Physical::actor()
{
	return *m_Actor;
}

PxRigidDynamic* Krimz::Physical::dynamic() const
{
	return m_Actor->is<PxRigidDynamic>();
}

void Krimz::Physical::dynamic(PxPhysics* physics, bool dynamic)
{
	if (dynamic != bool(this->dynamic()))
	{
		PxTransform originalTransform = m_Actor->getGlobalPose();
		m_Actor->release();
		if (dynamic)
		{
			m_Actor = physics->createRigidDynamic(originalTransform);
		}
		else
		{
			m_Actor = physics->createRigidStatic(originalTransform);
		}
	}
}

kl::float3 Krimz::Physical::rotation() const
{
	return transform().rotation;
}

void Krimz::Physical::rotation(const kl::float3& rotation)
{
	Transform transform = this->transform();
	transform.rotation = rotation;
	this->transform(transform);
}

kl::float3 Krimz::Physical::position() const
{
	return transform().position;
}

void Krimz::Physical::position(const kl::float3& position)
{
	Transform transform = this->transform();
	transform.position = position;
	this->transform(transform);
}

Krimz::Transform Krimz::Physical::transform() const
{
	return ConvertTransform(m_Actor->getGlobalPose());
}

void Krimz::Physical::transform(const Transform& transform)
{
	m_Actor->setGlobalPose(ConvertTransform(transform));
}

kl::float3 Krimz::Physical::velocity() const
{
	PxRigidDynamic* actor = dynamic();
	if (actor)
	{
		return Collider::ConvertVector(actor->getLinearVelocity());
	}
	return {};
}

void Krimz::Physical::velocity(const kl::float3& velocity)
{
	PxRigidDynamic* actor = dynamic();
	if (actor)
	{
		actor->setLinearVelocity(Collider::ConvertVector(velocity));
	}
}

kl::float3 Krimz::Physical::angular() const
{
	PxRigidDynamic* actor = dynamic();
	if (actor)
	{
		return kl::to::degrees(Collider::ConvertVector(actor->getAngularVelocity()));
	}
	return {};
}

void Krimz::Physical::angular(const kl::float3& angular)
{
	PxRigidDynamic* actor = dynamic();
	if (actor)
	{
		actor->setAngularVelocity(kl::to::radians(Collider::ConvertVector(angular)));
	}
}

bool Krimz::Physical::gravity() const
{
	PxActorFlags flags = m_Actor->getActorFlags();
	return !flags.isSet(PxActorFlag::eDISABLE_GRAVITY);
}

void Krimz::Physical::gravity(bool gravity)
{
	m_Actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !gravity);
}

float Krimz::Physical::mass() const
{
	PxRigidDynamic* actor = dynamic();
	if (actor)
	{
		return actor->getMass();
	}
	return 0.0f;
}

void Krimz::Physical::mass(float mass)
{
	PxRigidDynamic* actor = dynamic();
	if (actor)
	{
		actor->setMass(mass);
	}
}

void Krimz::Physical::attach(kl::ref<Collider> collider)
{
	m_Actor->attachShape(collider->shape());
}

void Krimz::Physical::detach(kl::ref<Collider> collider)
{
	m_Actor->detachShape(collider->shape());
}
