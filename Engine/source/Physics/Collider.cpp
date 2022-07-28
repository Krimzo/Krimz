#include "Physics/Collider.h"
#include "Physics/Physics.h"


PxVec3 Krimz::Collider::ConvertVector(const kl::float3& vector)
{
	return *(PxVec3*) &vector;
}

kl::float3 Krimz::Collider::ConvertVector(const PxVec3& vector)
{
	return *(kl::float3*) &vector;
}

Krimz::Collider::Collider(PxPhysics* physics, const physx::PxGeometry& geometry)
{
	m_Material = physics->createMaterial(0.0f, 0.0f, 0.0f);
	m_Shape = physics->createShape(geometry, *m_Material);
}

Krimz::Collider::Collider(PxPhysics* physics, const kl::float3& scale)
	: Collider(physics, PxBoxGeometry(ConvertVector(scale)))
{
}

Krimz::Collider::Collider(PxPhysics* physics, float radius)
	: Collider(physics, PxSphereGeometry(radius))
{
}

Krimz::Collider::Collider(PxPhysics* physics, float height, float radius)
	: Collider(physics, PxCapsuleGeometry(height, radius))
{
}

Krimz::Collider::Collider(PxPhysics* physics, kl::ref<Mesh> mesh, const kl::float3& scale)
	: Collider(physics, PxTriangleMeshGeometry(mesh->cooked, ConvertVector(scale)))
{
}

Krimz::Collider::~Collider()
{
	m_Shape->release();
	m_Material->release();
}

float Krimz::Collider::staticFriction() const
{
	return m_Material->getStaticFriction();
}

void Krimz::Collider::staticFriction(float friction)
{
	m_Material->setStaticFriction(friction);
}

float Krimz::Collider::dynamicFriction() const
{
	return m_Material->getDynamicFriction();
}

void Krimz::Collider::dynamicFriction(float friction)
{
	m_Material->setDynamicFriction(friction);
}

float Krimz::Collider::restitution() const
{
	return m_Material->getRestitution();
}

void Krimz::Collider::restitution(float restitution)
{
	m_Material->setRestitution(restitution);
}

PxShape& Krimz::Collider::shape() const
{
	return *m_Shape;
}

std::string Krimz::Collider::type() const
{
	switch (m_Shape->getGeometryType())
	{
	case PxGeometryType::eBOX:
		return "Box";
	case PxGeometryType::eSPHERE:
		return "Sphere";
	case PxGeometryType::eCAPSULE:
		return "Capsule";
	case PxGeometryType::eTRIANGLEMESH:
		return "Mesh";
	}
	return "Unknown";
}
