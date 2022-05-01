#include "Scripting/Scripting.h"
#include "Data/Entities.h"
#include "Utility/Time.h"


Engine::Script::Script(const String& filePath) : path(filePath) {
	reload();
}
Engine::Script::~Script() {
	Engine::JavaHandler::DelInst(inst);
}

// Reloads bytes
void Engine::Script::reload() {
	if (jclass scptCls = Engine::JavaHandler::LoadClass(path)) {
		Engine::JavaHandler::DelInst(inst);
		inst = Engine::JavaHandler::NewInst(scptCls, Engine::JavaHandler::GetMethod(scptCls, "<init>", "()V"));
		startMethod = Engine::JavaHandler::GetMethod(scptCls, "start", "()V");
		updateMethod = Engine::JavaHandler::GetMethod(scptCls, "update", "()V");
	}
}

// Data editors
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
		// Cast
		Engine::Entity* ent = (Engine::Entity*)entAddr;

		// Name
		jstring entName = Engine::JavaHandler::env->NewStringUTF(ent->getName().c_str());
		Engine::JavaHandler::env->SetObjectField(inst, Engine::JavaHandler::nameField, entName);
		Engine::JavaHandler::env->DeleteLocalRef(entName);

		// View		
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::visibleField, ent->visible);
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::shadowsField, ent->shadows);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::roughnessField, ent->roughness);

		// Geometry
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::scaleField), ent->scale);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::rotationField), ent->rotation);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::positionField), ent->position);

		// Physics
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::dynamicField, ent->dynamic);
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::gravityField, ent->gravity);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::frictionField, ent->friction);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::massField, ent->mass);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::velocityField), ent->velocity);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::angularField), ent->angular);

		// Collider
		jobject collider = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::colliderField);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collScaleField), ent->collider.scale);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collRotationField), ent->collider.rotation);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collPositionField), ent->collider.position);
		Engine::JavaHandler::env->SetIntField(collider, Engine::JavaHandler::collShapeField, int(ent->collider.shape));

		// Mesh/Texture
		Engine::JavaHandler::env->SetObjectField(inst, Engine::JavaHandler::meshField, Engine::JavaHandler::env->NewStringUTF(ent->mesh->getName().c_str()));
		Engine::JavaHandler::env->SetObjectField(inst, Engine::JavaHandler::textureField, Engine::JavaHandler::env->NewStringUTF(ent->texture->getName().c_str()));
	}
}
void Engine::Script::getEntityData(void* entAddr) {
	if (inst) {
		// Cast
		Engine::Entity* ent = (Engine::Entity*)entAddr;

		// View
		ent->visible = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::visibleField);
		ent->shadows = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::shadowsField);
		ent->roughness = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::roughnessField);

		// Geometry
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::scaleField), ent->scale);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::rotationField), ent->rotation);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::positionField), ent->position);

		// Physics
		ent->dynamic = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::dynamicField);
		ent->gravity = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::gravityField);
		ent->friction = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::frictionField);
		ent->mass = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::massField);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::velocityField), ent->velocity);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::angularField), ent->angular);

		// Collider
		jobject collider = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::colliderField);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collScaleField), ent->collider.scale);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collRotationField), ent->collider.rotation);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collPositionField), ent->collider.position);
		ent->collider.shape = Engine::Collider::Shape(Engine::JavaHandler::env->GetIntField(collider, Engine::JavaHandler::collShapeField));

		// Mesh/Texture
		const String scriptMesh = Engine::JavaHandler::env->GetStringUTFChars((jstring)Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::meshField), nullptr);
		if (scriptMesh != ent->mesh->getName()) {
			for (auto& mes : Engine::meshes) {
				if (mes->getName() == scriptMesh) {
					ent->mesh = mes.get();
					break;
				}
			}
		}
		const String scriptTexture = Engine::JavaHandler::env->GetStringUTFChars((jstring)Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::textureField), nullptr);
		if (scriptTexture != ent->texture->getName()) {
			for (auto& tex : Engine::textures) {
				if (tex->getName() == scriptTexture) {
					ent->texture = tex.get();
					break;
				}
			}
		}
	}
}

// Method callers
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
