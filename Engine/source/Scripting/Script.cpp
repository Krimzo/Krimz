#include "Scripting/Scripting.h"
#include "Types/Entity.h"
#include "Utility/Time.h"


Engine::Script::Script(const std::string& filePath) : path(filePath) {
	reload();
}
Engine::Script::~Script() {
	Engine::JavaHandler::DelInst(inst);
}

void Engine::Script::reload() {
	if (jclass scptCls = Engine::JavaHandler::LoadClass(path)) {
		Engine::JavaHandler::DelInst(inst);
		inst = Engine::JavaHandler::NewInst(scptCls, Engine::JavaHandler::GetMethod(scptCls, "<init>", "()V"));
		startMethod = Engine::JavaHandler::GetMethod(scptCls, "start", "()V");
		updateMethod = Engine::JavaHandler::GetMethod(scptCls, "update", "()V");
	}
}

void SetScriptFloat3(jobject field, const kl::float3& dat) {
	static jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "F");
	static jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "F");
	static jfieldID zField = Engine::JavaHandler::GetField(objCls, "z", "F");
	Engine::JavaHandler::env->SetFloatField(field, xField, dat.x);
	Engine::JavaHandler::env->SetFloatField(field, yField, dat.y);
	Engine::JavaHandler::env->SetFloatField(field, zField, dat.z);
}
void GetScriptFloat3(jobject field, kl::float3& dat) {
	static jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "F");
	static jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "F");
	static jfieldID zField = Engine::JavaHandler::GetField(objCls, "z", "F");
	dat.x = Engine::JavaHandler::env->GetFloatField(field, xField);
	dat.y = Engine::JavaHandler::env->GetFloatField(field, yField);
	dat.z = Engine::JavaHandler::env->GetFloatField(field, zField);
}
void Engine::Script::setEntityData(void* entAddr) {
	if (inst) {
		Engine::Entity* ent = (Engine::Entity*)entAddr;

		Engine::JavaHandler::env->SetObjectField(inst, Engine::JavaHandler::nameField, Engine::JavaHandler::env->NewStringUTF(ent->getName().c_str()));
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::visibleField, ent->visible);
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::shadowsField, ent->shadows);

		jobject material = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::materialField);
		Engine::JavaHandler::env->SetFloatField(material, Engine::JavaHandler::roughnessField, ent->material.roughness);
		Engine::JavaHandler::env->SetObjectField(material, Engine::JavaHandler::colorMapField, Engine::JavaHandler::env->NewStringUTF(ent->material.colorMap->getName().c_str()));
		Engine::JavaHandler::env->SetObjectField(material, Engine::JavaHandler::normalMapField, Engine::JavaHandler::env->NewStringUTF(ent->material.normalMap->getName().c_str()));
		Engine::JavaHandler::env->SetObjectField(material, Engine::JavaHandler::roughnessMapField, Engine::JavaHandler::env->NewStringUTF(ent->material.roughnessMap->getName().c_str()));

		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::scaleField), ent->scale);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::rotationField), ent->rotation);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::positionField), ent->position);
		Engine::JavaHandler::env->SetObjectField(inst, Engine::JavaHandler::meshField, Engine::JavaHandler::env->NewStringUTF(ent->mesh->getName().c_str()));

		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::dynamicField, ent->dynamic);
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::gravityField, ent->gravity);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::frictionField, ent->friction);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::massField, ent->mass);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::velocityField), ent->velocity);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::angularField), ent->angular);

		jobject collider = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::colliderField);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collScaleField), ent->collider.scale);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collRotationField), ent->collider.rotation);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collPositionField), ent->collider.position);
		Engine::JavaHandler::env->SetIntField(collider, Engine::JavaHandler::collShapeField, int(ent->collider.shape));
	}
}
void Engine::Script::getEntityData(void* entAddr) {
	if (inst) {
		Engine::Entity* ent = (Engine::Entity*)entAddr;

		ent->visible = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::visibleField);
		ent->shadows = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::shadowsField);

		jobject material = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::materialField);
		ent->material.roughness = Engine::JavaHandler::env->GetFloatField(material, Engine::JavaHandler::roughnessField);
		const std::string colorMapName = Engine::JavaHandler::env->GetStringUTFChars((jstring)Engine::JavaHandler::env->GetObjectField(material, Engine::JavaHandler::colorMapField), nullptr);
		if (colorMapName != ent->material.colorMap->getName()) {
			for (auto& tex : Engine::textures) {
				if (tex->getName() == colorMapName) {
					ent->material.colorMap = tex;
					break;
				}
			}
		}
		const std::string normalMapName = Engine::JavaHandler::env->GetStringUTFChars((jstring)Engine::JavaHandler::env->GetObjectField(material, Engine::JavaHandler::normalMapField), nullptr);
		if (normalMapName != ent->material.normalMap->getName()) {
			for (auto& tex : Engine::textures) {
				if (tex->getName() == normalMapName) {
					ent->material.normalMap = tex;
					break;
				}
			}
		}
		const std::string roughnessMapName = Engine::JavaHandler::env->GetStringUTFChars((jstring)Engine::JavaHandler::env->GetObjectField(material, Engine::JavaHandler::roughnessMapField), nullptr);
		if (roughnessMapName != ent->material.roughnessMap->getName()) {
			for (auto& tex : Engine::textures) {
				if (tex->getName() == roughnessMapName) {
					ent->material.roughnessMap = tex;
					break;
				}
			}
		}

		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::scaleField), ent->scale);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::rotationField), ent->rotation);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::positionField), ent->position);
		const std::string meshName = Engine::JavaHandler::env->GetStringUTFChars((jstring)Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::meshField), nullptr);
		if (meshName != ent->mesh->getName()) {
			for (auto& mes : Engine::meshes) {
				if (mes->getName() == meshName) {
					ent->mesh = mes;
					break;
				}
			}
		}

		ent->dynamic = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::dynamicField);
		ent->gravity = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::gravityField);
		ent->friction = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::frictionField);
		ent->mass = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::massField);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::velocityField), ent->velocity);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::angularField), ent->angular);

		jobject collider = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::colliderField);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collScaleField), ent->collider.scale);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collRotationField), ent->collider.rotation);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collPositionField), ent->collider.position);
		ent->collider.shape = Engine::Collider::Shape(Engine::JavaHandler::env->GetIntField(collider, Engine::JavaHandler::collShapeField));
	}
}

void Engine::Script::callStart(void* ent) {
	if (inst) {
		setEntityData(ent);
		Engine::JavaHandler::env->CallVoidMethod(inst, startMethod);
		getEntityData(ent);
	}
}
void Engine::Script::callUpdate(void* ent) {
	if (inst) {
		setEntityData(ent);
		Engine::JavaHandler::env->CallVoidMethod(inst, updateMethod);
		getEntityData(ent);
	}
}
