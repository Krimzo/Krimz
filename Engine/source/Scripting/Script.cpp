#include "Scripting/Scripting.h"
#include "Data/Entities.h"
#include "Utility/Time.h"


Engine::Script::Script(const std::string& filePath) : path(filePath) {
	reload();
}

// Reloads bytes
void Engine::Script::reload() {
	if (jclass scptCls = Engine::JavaHandler::LoadClass(path)) {
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
void Engine::Script::setEntityData(void* ent) {
	if (inst) {
		// Cast
		Engine::Entity* pEnt = (Engine::Entity*)ent;

		// View
		Engine::JavaHandler::env->SetObjectField(inst, Engine::JavaHandler::nameField, Engine::JavaHandler::env->NewString((jchar*)kl::convert::toWString(pEnt->name).c_str(), jsize(pEnt->name.size())));
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::visibleField, pEnt->visible);
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::shadowsField, pEnt->shadows);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::roughnessField, pEnt->roughness);

		// Geometry
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::scaleField), pEnt->scale);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::rotationField), pEnt->rotation);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::positionField), pEnt->position);

		// Physics
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::dynamicField, pEnt->dynamic);
		Engine::JavaHandler::env->SetBooleanField(inst, Engine::JavaHandler::gravityField, pEnt->gravity);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::frictionField, pEnt->friction);
		Engine::JavaHandler::env->SetFloatField(inst, Engine::JavaHandler::massField, pEnt->mass);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::velocityField), pEnt->velocity);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::angularField), pEnt->angular);

		// Collider
		jobject collider = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::colliderField);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collScaleField), pEnt->collider.scale);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collRotationField), pEnt->collider.rotation);
		SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collPositionField), pEnt->collider.position);
		Engine::JavaHandler::env->SetIntField(collider, Engine::JavaHandler::collShapeField, int(pEnt->collider.shape));
	}
}
void Engine::Script::getEntityData(void* ent) {
	if (inst) {
		// Cast
		Engine::Entity* pEnt = (Engine::Entity*)ent;

		// View
		pEnt->visible = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::visibleField);
		pEnt->shadows = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::shadowsField);
		pEnt->roughness = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::roughnessField);

		// Geometry
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::scaleField), pEnt->scale);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::rotationField), pEnt->rotation);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::positionField), pEnt->position);

		// Physics
		pEnt->dynamic = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::dynamicField);
		pEnt->gravity = Engine::JavaHandler::env->GetBooleanField(inst, Engine::JavaHandler::gravityField);
		pEnt->friction = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::frictionField);
		pEnt->mass = Engine::JavaHandler::env->GetFloatField(inst, Engine::JavaHandler::massField);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::velocityField), pEnt->velocity);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::angularField), pEnt->angular);

		// Collider
		jobject collider = Engine::JavaHandler::env->GetObjectField(inst, Engine::JavaHandler::colliderField);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collScaleField), pEnt->collider.scale);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collRotationField), pEnt->collider.rotation);
		GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(collider, Engine::JavaHandler::collPositionField), pEnt->collider.position);
		pEnt->collider.shape = Engine::Collider::Shape(Engine::JavaHandler::env->GetIntField(collider, Engine::JavaHandler::collShapeField));
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
