#include "Scripting/Scripting.h"
#include "Data/Entities.h"
#include "Utility/Time.h"


Engine::Script::Script(const std::string& filePath)
{
	this->path = filePath;
	reload();
}

// Reloads bytes
void Engine::Script::reload()
{
	if (jclass cls = Engine::JavaHandler::LoadClass(path))
	{
		// Instance
		inst = Engine::JavaHandler::NewInst(cls, Engine::JavaHandler::GetMethod(cls, "<init>", "()V"));

		// Methods
		startMethod = Engine::JavaHandler::GetMethod(cls, "start", "()V");
		updateMethod = Engine::JavaHandler::GetMethod(cls, "update", "()V");

		// Fields
		nameField = Engine::JavaHandler::GetField(cls, "name", "Ljava/lang/String;");
		visibleField = Engine::JavaHandler::GetField(cls, "visible", "Z");
		shadowsField = Engine::JavaHandler::GetField(cls, "shadows", "Z");
		roughnessField = Engine::JavaHandler::GetField(cls, "roughness", "F");
		scaleField = Engine::JavaHandler::GetField(cls, "scale", "Lengine/math/Float3;");
		rotationField = Engine::JavaHandler::GetField(cls, "rotation", "Lengine/math/Float3;");
		positionField = Engine::JavaHandler::GetField(cls, "position", "Lengine/math/Float3;");
		physicsField = Engine::JavaHandler::GetField(cls, "physics", "Z");
		accelerField = Engine::JavaHandler::GetField(cls, "acceler", "Lengine/math/Float3;");
		velocityField = Engine::JavaHandler::GetField(cls, "velocity", "Lengine/math/Float3;");
		angularField = Engine::JavaHandler::GetField(cls, "angular", "Lengine/math/Float3;");

		// State set
		loaded = true;
	}
	else
		loaded = false;
}

// Data editors
void SetScriptFloat3(jobject field, const kl::float3& dat)
{
	jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "F");
	jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "F");
	jfieldID zField = Engine::JavaHandler::GetField(objCls, "z", "F");
	Engine::JavaHandler::env->SetFloatField(field, xField, dat.x);
	Engine::JavaHandler::env->SetFloatField(field, yField, dat.y);
	Engine::JavaHandler::env->SetFloatField(field, zField, dat.z);
}
void GetScriptFloat3(jobject field, kl::float3& dat)
{
	jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "F");
	jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "F");
	jfieldID zField = Engine::JavaHandler::GetField(objCls, "z", "F");
	dat.x = Engine::JavaHandler::env->GetFloatField(field, xField);
	dat.y = Engine::JavaHandler::env->GetFloatField(field, yField);
	dat.z = Engine::JavaHandler::env->GetFloatField(field, zField);
}
void Engine::Script::setEntityData(void* ent)
{
	// Check
	if (!loaded)
		return;

	// Work
	Engine::Entity* pEnt = (Engine::Entity*)ent;
	Engine::JavaHandler::env->SetObjectField(inst, nameField,
		Engine::JavaHandler::env->NewString(
			(jchar*)kl::convert::toWString(pEnt->name).c_str(), jsize(pEnt->name.size())));
	Engine::JavaHandler::env->SetBooleanField(inst, visibleField, pEnt->visible);
	Engine::JavaHandler::env->SetBooleanField(inst, shadowsField, pEnt->shadows);
	Engine::JavaHandler::env->SetFloatField(inst, roughnessField, pEnt->roughness);
	SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, scaleField), pEnt->scale);
	SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, rotationField), pEnt->rotation);
	SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, positionField), pEnt->position);
	Engine::JavaHandler::env->SetBooleanField(inst, physicsField, pEnt->physics);
	SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, accelerField), pEnt->acceler);
	SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, velocityField), pEnt->velocity);
	SetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, angularField), pEnt->angular);
}
void Engine::Script::getEntityData(void* ent)
{
	// Check
	if (!loaded)
		return;

	// Work
	Engine::Entity* pEnt = (Engine::Entity*)ent;
	pEnt->visible = Engine::JavaHandler::env->GetBooleanField(inst, visibleField);
	pEnt->shadows = Engine::JavaHandler::env->GetBooleanField(inst, shadowsField);
	pEnt->roughness = Engine::JavaHandler::env->GetFloatField(inst, roughnessField);
	GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, scaleField), pEnt->scale);
	GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, rotationField), pEnt->rotation);
	GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, positionField), pEnt->position);
	pEnt->physics = Engine::JavaHandler::env->GetBooleanField(inst, physicsField);
	GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, accelerField), pEnt->acceler);
	GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, velocityField), pEnt->velocity);
	GetScriptFloat3(Engine::JavaHandler::env->GetObjectField(inst, angularField), pEnt->angular);
}

// Method callers
void Engine::Script::callStart(void* ent)
{
	// Check
	if (!loaded)
		return;

	// Work
	setEntityData(ent);
	Engine::JavaHandler::env->CallVoidMethod(inst, startMethod);
	getEntityData(ent);
}
void Engine::Script::callUpdate(void* ent)
{
	// Check
	if (!loaded)
		return;

	// Work
	setEntityData(ent);
	Engine::JavaHandler::env->CallVoidMethod(inst, updateMethod);
	getEntityData(ent);
}
