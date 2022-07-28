#include "Input/Input.h"
#include "Render/Render.h"
#include "Editor/Picking.h"
#include "Editor/Gizmo.h"
#include "Types/Camera.h"
#include "Types/Entity.h"
#include "GUI/GUI.h"


static kl::float3 offsPos;
static kl::float3 firstScal;
static kl::float3 offsRot;
static kl::float3 firstRota;

static const kl::float3 f3PosX = { 1.0f, 0.0f, 0.0f };
static const kl::float3 f3PosY = { 0.0f, 1.0f, 0.0f };
static const kl::float3 f3PosZ = { 0.0f, 0.0f, 1.0f };
static const kl::float2 f2PosX = { 1.0f, 0.0f };

kl::ray GetMouseRay()
{
	kl::float2 mouseUV = Krimz::window.mouse.position;
	mouseUV -= Krimz::GUI::viewportPosition;
	mouseUV = kl::float2(mouseUV.x / Krimz::GUI::viewportSize.x, (Krimz::GUI::viewportSize.y - mouseUV.y) / Krimz::GUI::viewportSize.y);
	mouseUV *= 2.0f;
	mouseUV -= 1.0f;

	kl::float4 farMousePoint = Krimz::Selected::camera->matrix().inverse() * kl::float4(mouseUV, 1.0f, 1.0f);
	farMousePoint /= farMousePoint.w;

	return kl::ray(Krimz::Selected::camera->position, farMousePoint.xyz - Krimz::Selected::camera->position);
}

void IntersectPlanes(const kl::ray& ray, kl::float3& out1, kl::float3& out2, kl::float3& out3)
{
	const kl::float3 camFor = Krimz::Selected::camera->forward();
	ray.intersect(kl::plane(f3PosY, Krimz::Selected::entity->position), &out1);
	ray.intersect(kl::plane((abs(camFor.x) > abs(camFor.z) ? f3PosX : f3PosZ), Krimz::Selected::entity->position), &out2);
	ray.intersect(kl::plane(f3PosY, Krimz::Selected::entity->position), &out3);
}

void LMBPress()
{
	if (Krimz::GUI::viewportFocus)
	{
		if (Krimz::Picking::mouseIndex >= 0)
		{
			auto entIt = Krimz::entities.begin();
			std::advance(entIt, Krimz::Picking::mouseIndex);
			Krimz::Selected::entity = *entIt;
			Krimz::Picking::selectedIndex = Krimz::Picking::mouseIndex;
		}
		else if (Krimz::Picking::mouseIndex == -1)
		{
			Krimz::Selected::entity = nullptr;
			Krimz::Picking::selectedIndex = -1;
		}

		if (Krimz::Selected::entity)
		{
			firstScal = Krimz::Selected::entity->scale;
			firstRota = Krimz::Selected::entity->rotation;
			Krimz::Picking::heldIndex = Krimz::Picking::mouseIndex;

			const kl::ray mouseRay = GetMouseRay();
			kl::float3 inter1, inter2, inter3;
			IntersectPlanes(mouseRay, inter1, inter2, inter3);
			offsPos.x = inter1.x - Krimz::Selected::entity->position.x;
			offsPos.y = inter2.y - Krimz::Selected::entity->position.y;
			offsPos.z = inter3.z - Krimz::Selected::entity->position.z;

			kl::float3 intersectX;
			mouseRay.intersect(kl::plane(f3PosX, Krimz::Selected::entity->position), &intersectX);
			kl::float2 secondVecX = kl::float2(intersectX.y, intersectX.z) - kl::float2(Krimz::Selected::entity->position.y, Krimz::Selected::entity->position.z);
			offsRot.x = f2PosX.angle(secondVecX, true);

			kl::float3 intersectY;
			mouseRay.intersect(kl::plane(f3PosY, Krimz::Selected::entity->position), &intersectY);
			kl::float2 secondVecY = kl::float2(intersectY.z, intersectY.x) - kl::float2(Krimz::Selected::entity->position.z, Krimz::Selected::entity->position.x);
			offsRot.y = f2PosX.angle(secondVecY, true);

			kl::float3 intersectZ;
			mouseRay.intersect(kl::plane(f3PosZ, Krimz::Selected::entity->position), &intersectZ);
			kl::float2 secondVecZ = kl::float2(intersectZ.x, intersectZ.y) - kl::float2(Krimz::Selected::entity->position.x, Krimz::Selected::entity->position.y);
			offsRot.z = f2PosX.angle(secondVecZ, true);
		}
	}
}

void LMBDown()
{
	if (Krimz::Selected::entity)
	{
		kl::float3 inter1, inter2, inter3;
		const kl::ray mouseRay = GetMouseRay();
		IntersectPlanes(mouseRay, inter1, inter2, inter3);

		const kl::float3 currPos = kl::float3(inter1.x, inter2.y, inter3.z) - offsPos;

		if (Krimz::Selected::gizmo == Krimz::Gizmo::Type::SCALE)
		{
			if (Krimz::Picking::heldIndex == -3)
			{
				Krimz::Selected::entity->scale.x = firstScal.x + (currPos.x - Krimz::Selected::entity->position.x);
			}
			else if (Krimz::Picking::heldIndex == -4)
			{
				Krimz::Selected::entity->scale.y = firstScal.y + (currPos.y - Krimz::Selected::entity->position.y);
			}
			else if (Krimz::Picking::heldIndex == -5)
			{
				Krimz::Selected::entity->scale.z = firstScal.z + (currPos.z - Krimz::Selected::entity->position.z);
			}
		}
		else if (Krimz::Selected::gizmo == Krimz::Gizmo::Type::MOVE)
		{
			if (Krimz::Picking::heldIndex == -3)
			{
				Krimz::Selected::entity->position.x = currPos.x;
			}
			else if (Krimz::Picking::heldIndex == -4)
			{
				Krimz::Selected::entity->position.y = currPos.y;
			}
			else if (Krimz::Picking::heldIndex == -5)
			{
				Krimz::Selected::entity->position.z = currPos.z;
			}
		}
		else if (Krimz::Selected::gizmo == Krimz::Gizmo::Type::ROTATE)
		{
			if (Krimz::Picking::heldIndex == -3)
			{
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(f3PosX, Krimz::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.y, intersect.z) - kl::float2(Krimz::Selected::entity->position.y, Krimz::Selected::entity->position.z);
				Krimz::Selected::entity->rotation.x = firstRota.x + f2PosX.angle(secondVec, true) - offsRot.x;
			}
			else if (Krimz::Picking::heldIndex == -4)
			{
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(f3PosY, Krimz::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.z, intersect.x) - kl::float2(Krimz::Selected::entity->position.z, Krimz::Selected::entity->position.x);
				Krimz::Selected::entity->rotation.y = firstRota.y + f2PosX.angle(secondVec, true) - offsRot.y;
			}
			else if (Krimz::Picking::heldIndex == -5)
			{
				kl::float3 intersect;
				mouseRay.intersect(kl::plane(f3PosZ, Krimz::Selected::entity->position), &intersect);
				kl::float2 secondVec = kl::float2(intersect.x, intersect.y) - kl::float2(Krimz::Selected::entity->position.x, Krimz::Selected::entity->position.y);
				Krimz::Selected::entity->rotation.z = firstRota.z + f2PosX.angle(secondVec, true) - offsRot.z;
			}
		}
	}
}

void LMBRelease()
{
	Krimz::Picking::heldIndex = -1;
}

void Krimz::Input::LMB()
{
	Krimz::window.mouse.lmb.press = LMBPress;
	Krimz::window.mouse.lmb.down = LMBDown;
	Krimz::window.mouse.lmb.release = LMBRelease;
}
