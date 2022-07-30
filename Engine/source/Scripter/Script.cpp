#include "Scripter/Script.h"


Krimz::Script::Script(const std::string& filePath) : m_Path(filePath) {

}
Krimz::Script::~Script() {

}

/*
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
	static jclass objCls = Scripter::ENV->GetObjectClass(field);
	static jfieldID xField = Krimz::JavaHandler::GetField(objCls, "x", "F");
	static jfieldID yField = Krimz::JavaHandler::GetField(objCls, "y", "F");
	static jfieldID zField = Krimz::JavaHandler::GetField(objCls, "z", "F");
	Scripter::ENV->SetFloatField(field, xField, dat.x);
	Scripter::ENV->SetFloatField(field, yField, dat.y);
	Scripter::ENV->SetFloatField(field, zField, dat.z);
}
void GetScriptFloat3(jobject field, kl::float3& dat)
{
	static jclass objCls = Scripter::ENV->GetObjectClass(field);
	static jfieldID xField = Krimz::JavaHandler::GetField(objCls, "x", "F");
	static jfieldID yField = Krimz::JavaHandler::GetField(objCls, "y", "F");
	static jfieldID zField = Krimz::JavaHandler::GetField(objCls, "z", "F");
	dat.x = Scripter::ENV->GetFloatField(field, xField);
	dat.y = Scripter::ENV->GetFloatField(field, yField);
	dat.z = Scripter::ENV->GetFloatField(field, zField);
}
void Krimz::Script::setEntityData(void* entAddr)
{
	if (inst)
	{
		Krimz::Entity* ent = (Krimz::Entity*) entAddr;

		Scripter::ENV->SetObjectField(inst, Krimz::JavaHandler::nameField, Scripter::ENV->NewStringUTF(ent->getName().c_str()));
		Scripter::ENV->SetBooleanField(inst, Krimz::JavaHandler::visibleField, ent->visible);
		Scripter::ENV->SetBooleanField(inst, Krimz::JavaHandler::shadowsField, ent->shadows);

		jobject material = Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::materialField);
		Scripter::ENV->SetFloatField(material, Krimz::JavaHandler::roughnessField, ent->material.roughness);
		Scripter::ENV->SetObjectField(material, Krimz::JavaHandler::colorMapField, Scripter::ENV->NewStringUTF(ent->material.colorMap->getName().c_str()));
		Scripter::ENV->SetObjectField(material, Krimz::JavaHandler::normalMapField, Scripter::ENV->NewStringUTF(ent->material.normalMap->getName().c_str()));
		Scripter::ENV->SetObjectField(material, Krimz::JavaHandler::roughnessMapField, Scripter::ENV->NewStringUTF(ent->material.roughnessMap->getName().c_str()));

		SetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::scaleField), ent->scale);
		SetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::rotationField), ent->rotation);
		SetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::positionField), ent->position);
		Scripter::ENV->SetObjectField(inst, Krimz::JavaHandler::meshField, Scripter::ENV->NewStringUTF(ent->mesh->getName().c_str()));

		Scripter::ENV->SetBooleanField(inst, Krimz::JavaHandler::dynamicField, ent->dynamic);
		Scripter::ENV->SetBooleanField(inst, Krimz::JavaHandler::gravityField, ent->gravity);
		Scripter::ENV->SetFloatField(inst, Krimz::JavaHandler::frictionField, ent->friction);
		Scripter::ENV->SetFloatField(inst, Krimz::JavaHandler::massField, ent->mass);
		SetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::velocityField), ent->velocity);
		SetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::angularField), ent->angular);

		jobject collider = Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::colliderField);
		SetScriptFloat3(Scripter::ENV->GetObjectField(collider, Krimz::JavaHandler::collScaleField), ent->collider.scale);
		SetScriptFloat3(Scripter::ENV->GetObjectField(collider, Krimz::JavaHandler::collRotationField), ent->collider.rotation);
		SetScriptFloat3(Scripter::ENV->GetObjectField(collider, Krimz::JavaHandler::collPositionField), ent->collider.position);
		Scripter::ENV->SetIntField(collider, Krimz::JavaHandler::collShapeField, int(ent->collider.shape));
	}
}
void Krimz::Script::getEntityData(void* entAddr)
{
	if (inst)
	{
		Krimz::Entity* ent = (Krimz::Entity*) entAddr;

		ent->visible = Scripter::ENV->GetBooleanField(inst, Krimz::JavaHandler::visibleField);
		ent->shadows = Scripter::ENV->GetBooleanField(inst, Krimz::JavaHandler::shadowsField);

		jobject material = Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::materialField);
		ent->material.roughness = Scripter::ENV->GetFloatField(material, Krimz::JavaHandler::roughnessField);
		const std::string colorMapName = Scripter::ENV->GetStringUTFChars((jstring) Scripter::ENV->GetObjectField(material, Krimz::JavaHandler::colorMapField), nullptr);
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
		const std::string normalMapName = Scripter::ENV->GetStringUTFChars((jstring) Scripter::ENV->GetObjectField(material, Krimz::JavaHandler::normalMapField), nullptr);
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
		const std::string roughnessMapName = Scripter::ENV->GetStringUTFChars((jstring) Scripter::ENV->GetObjectField(material, Krimz::JavaHandler::roughnessMapField), nullptr);
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

		GetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::scaleField), ent->scale);
		GetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::rotationField), ent->rotation);
		GetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::positionField), ent->position);
		const std::string meshName = Scripter::ENV->GetStringUTFChars((jstring) Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::meshField), nullptr);
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

		ent->dynamic = Scripter::ENV->GetBooleanField(inst, Krimz::JavaHandler::dynamicField);
		ent->gravity = Scripter::ENV->GetBooleanField(inst, Krimz::JavaHandler::gravityField);
		ent->friction = Scripter::ENV->GetFloatField(inst, Krimz::JavaHandler::frictionField);
		ent->mass = Scripter::ENV->GetFloatField(inst, Krimz::JavaHandler::massField);
		GetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::velocityField), ent->velocity);
		GetScriptFloat3(Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::angularField), ent->angular);

		jobject collider = Scripter::ENV->GetObjectField(inst, Krimz::JavaHandler::colliderField);
		GetScriptFloat3(Scripter::ENV->GetObjectField(collider, Krimz::JavaHandler::collScaleField), ent->collider.scale);
		GetScriptFloat3(Scripter::ENV->GetObjectField(collider, Krimz::JavaHandler::collRotationField), ent->collider.rotation);
		GetScriptFloat3(Scripter::ENV->GetObjectField(collider, Krimz::JavaHandler::collPositionField), ent->collider.position);
		ent->collider.shape = Krimz::Collider::Shape(Scripter::ENV->GetIntField(collider, Krimz::JavaHandler::collShapeField));
	}
}

void Krimz::Script::callStart(void* ent)
{
	if (inst)
	{
		setEntityData(ent);
		Scripter::ENV->CallVoidMethod(inst, startMethod);
		getEntityData(ent);
	}
}
void Krimz::Script::callUpdate(void* ent)
{
	if (inst)
	{
		setEntityData(ent);
		Scripter::ENV->CallVoidMethod(inst, updateMethod);
		getEntityData(ent);
	}
}
*/
