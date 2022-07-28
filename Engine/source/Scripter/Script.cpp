#include "Scripting/Scripting.h"
#include "Types/Entity.h"
#include "Utility/Time.h"


Krimz::Script::Script(const std::string& filePath) : path(filePath)
{
	reload();
}
Krimz::Script::~Script()
{
	Krimz::JavaHandler::DelInst(inst);
}

void Krimz::Script::reload()
{
	if (jclass scptCls = Krimz::JavaHandler::LoadClass(path))
	{
		Krimz::JavaHandler::DelInst(inst);
		inst = Krimz::JavaHandler::NewInst(scptCls, Krimz::JavaHandler::GetMethod(scptCls, "<init>", "()V"));
		startMethod = Krimz::JavaHandler::GetMethod(scptCls, "start", "()V");
		updateMethod = Krimz::JavaHandler::GetMethod(scptCls, "update", "()V");
	}
}

void SetScriptFloat3(jobject field, const kl::float3& dat)
{
	static jclass objCls = Krimz::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Krimz::JavaHandler::GetField(objCls, "x", "F");
	static jfieldID yField = Krimz::JavaHandler::GetField(objCls, "y", "F");
	static jfieldID zField = Krimz::JavaHandler::GetField(objCls, "z", "F");
	Krimz::JavaHandler::env->SetFloatField(field, xField, dat.x);
	Krimz::JavaHandler::env->SetFloatField(field, yField, dat.y);
	Krimz::JavaHandler::env->SetFloatField(field, zField, dat.z);
}
void GetScriptFloat3(jobject field, kl::float3& dat)
{
	static jclass objCls = Krimz::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Krimz::JavaHandler::GetField(objCls, "x", "F");
	static jfieldID yField = Krimz::JavaHandler::GetField(objCls, "y", "F");
	static jfieldID zField = Krimz::JavaHandler::GetField(objCls, "z", "F");
	dat.x = Krimz::JavaHandler::env->GetFloatField(field, xField);
	dat.y = Krimz::JavaHandler::env->GetFloatField(field, yField);
	dat.z = Krimz::JavaHandler::env->GetFloatField(field, zField);
}
void Krimz::Script::setEntityData(void* entAddr)
{
	if (inst)
	{
		Krimz::Entity* ent = (Krimz::Entity*) entAddr;

		Krimz::JavaHandler::env->SetObjectField(inst, Krimz::JavaHandler::nameField, Krimz::JavaHandler::env->NewStringUTF(ent->getName().c_str()));
		Krimz::JavaHandler::env->SetBooleanField(inst, Krimz::JavaHandler::visibleField, ent->visible);
		Krimz::JavaHandler::env->SetBooleanField(inst, Krimz::JavaHandler::shadowsField, ent->shadows);

		jobject material = Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::materialField);
		Krimz::JavaHandler::env->SetFloatField(material, Krimz::JavaHandler::roughnessField, ent->material.roughness);
		Krimz::JavaHandler::env->SetObjectField(material, Krimz::JavaHandler::colorMapField, Krimz::JavaHandler::env->NewStringUTF(ent->material.colorMap->getName().c_str()));
		Krimz::JavaHandler::env->SetObjectField(material, Krimz::JavaHandler::normalMapField, Krimz::JavaHandler::env->NewStringUTF(ent->material.normalMap->getName().c_str()));
		Krimz::JavaHandler::env->SetObjectField(material, Krimz::JavaHandler::roughnessMapField, Krimz::JavaHandler::env->NewStringUTF(ent->material.roughnessMap->getName().c_str()));

		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::scaleField), ent->scale);
		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::rotationField), ent->rotation);
		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::positionField), ent->position);
		Krimz::JavaHandler::env->SetObjectField(inst, Krimz::JavaHandler::meshField, Krimz::JavaHandler::env->NewStringUTF(ent->mesh->getName().c_str()));

		Krimz::JavaHandler::env->SetBooleanField(inst, Krimz::JavaHandler::dynamicField, ent->dynamic);
		Krimz::JavaHandler::env->SetBooleanField(inst, Krimz::JavaHandler::gravityField, ent->gravity);
		Krimz::JavaHandler::env->SetFloatField(inst, Krimz::JavaHandler::frictionField, ent->friction);
		Krimz::JavaHandler::env->SetFloatField(inst, Krimz::JavaHandler::massField, ent->mass);
		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::velocityField), ent->velocity);
		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::angularField), ent->angular);

		jobject collider = Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::colliderField);
		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(collider, Krimz::JavaHandler::collScaleField), ent->collider.scale);
		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(collider, Krimz::JavaHandler::collRotationField), ent->collider.rotation);
		SetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(collider, Krimz::JavaHandler::collPositionField), ent->collider.position);
		Krimz::JavaHandler::env->SetIntField(collider, Krimz::JavaHandler::collShapeField, int(ent->collider.shape));
	}
}
void Krimz::Script::getEntityData(void* entAddr)
{
	if (inst)
	{
		Krimz::Entity* ent = (Krimz::Entity*) entAddr;

		ent->visible = Krimz::JavaHandler::env->GetBooleanField(inst, Krimz::JavaHandler::visibleField);
		ent->shadows = Krimz::JavaHandler::env->GetBooleanField(inst, Krimz::JavaHandler::shadowsField);

		jobject material = Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::materialField);
		ent->material.roughness = Krimz::JavaHandler::env->GetFloatField(material, Krimz::JavaHandler::roughnessField);
		const std::string colorMapName = Krimz::JavaHandler::env->GetStringUTFChars((jstring) Krimz::JavaHandler::env->GetObjectField(material, Krimz::JavaHandler::colorMapField), nullptr);
		if (colorMapName != ent->material.colorMap->getName())
		{
			for (auto& tex : Krimz::textures)
			{
				if (tex->getName() == colorMapName)
				{
					ent->material.colorMap = tex;
					break;
				}
			}
		}
		const std::string normalMapName = Krimz::JavaHandler::env->GetStringUTFChars((jstring) Krimz::JavaHandler::env->GetObjectField(material, Krimz::JavaHandler::normalMapField), nullptr);
		if (normalMapName != ent->material.normalMap->getName())
		{
			for (auto& tex : Krimz::textures)
			{
				if (tex->getName() == normalMapName)
				{
					ent->material.normalMap = tex;
					break;
				}
			}
		}
		const std::string roughnessMapName = Krimz::JavaHandler::env->GetStringUTFChars((jstring) Krimz::JavaHandler::env->GetObjectField(material, Krimz::JavaHandler::roughnessMapField), nullptr);
		if (roughnessMapName != ent->material.roughnessMap->getName())
		{
			for (auto& tex : Krimz::textures)
			{
				if (tex->getName() == roughnessMapName)
				{
					ent->material.roughnessMap = tex;
					break;
				}
			}
		}

		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::scaleField), ent->scale);
		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::rotationField), ent->rotation);
		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::positionField), ent->position);
		const std::string meshName = Krimz::JavaHandler::env->GetStringUTFChars((jstring) Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::meshField), nullptr);
		if (meshName != ent->mesh->getName())
		{
			for (auto& mes : Krimz::meshes)
			{
				if (mes->getName() == meshName)
				{
					ent->mesh = mes;
					break;
				}
			}
		}

		ent->dynamic = Krimz::JavaHandler::env->GetBooleanField(inst, Krimz::JavaHandler::dynamicField);
		ent->gravity = Krimz::JavaHandler::env->GetBooleanField(inst, Krimz::JavaHandler::gravityField);
		ent->friction = Krimz::JavaHandler::env->GetFloatField(inst, Krimz::JavaHandler::frictionField);
		ent->mass = Krimz::JavaHandler::env->GetFloatField(inst, Krimz::JavaHandler::massField);
		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::velocityField), ent->velocity);
		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::angularField), ent->angular);

		jobject collider = Krimz::JavaHandler::env->GetObjectField(inst, Krimz::JavaHandler::colliderField);
		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(collider, Krimz::JavaHandler::collScaleField), ent->collider.scale);
		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(collider, Krimz::JavaHandler::collRotationField), ent->collider.rotation);
		GetScriptFloat3(Krimz::JavaHandler::env->GetObjectField(collider, Krimz::JavaHandler::collPositionField), ent->collider.position);
		ent->collider.shape = Krimz::Collider::Shape(Krimz::JavaHandler::env->GetIntField(collider, Krimz::JavaHandler::collShapeField));
	}
}

void Krimz::Script::callStart(void* ent)
{
	if (inst)
	{
		setEntityData(ent);
		Krimz::JavaHandler::env->CallVoidMethod(inst, startMethod);
		getEntityData(ent);
	}
}
void Krimz::Script::callUpdate(void* ent)
{
	if (inst)
	{
		setEntityData(ent);
		Krimz::JavaHandler::env->CallVoidMethod(inst, updateMethod);
		getEntityData(ent);
	}
}
