#include "Scripting/Scripting.h"
#include "Data/Entities.h"
#include "Utility/Time.h"


Engine::Script::Script(const std::string& name, const std::string& filePath) {
	// Saving file info
	this->name = name;
	this->path = filePath;

	// Loading data
	reload();
}
Engine::Script::~Script() {
	Engine::Handler::DelInst(inst);
}

// Reloads byte data
void Engine::Script::reload() {
	// Loading class from file
	jclass cls = Engine::Handler::LoadClass(name, path);

	// Instance
	if (inst) Engine::Handler::DelInst(inst);
	inst = Engine::Handler::NewInst(cls, Engine::Handler::GetMethod(cls, "<init>", "()V"));

	// Methods
	startMethod = Engine::Handler::GetMethod(cls, "start", "()V");
	updateMethod = Engine::Handler::GetMethod(cls, "update", "()V");

	// Fields
	nameField = Engine::Handler::GetField(cls, "name", "Ljava/lang/String;");
	visibleField = Engine::Handler::GetField(cls, "visible", "Z");
	shadowsField = Engine::Handler::GetField(cls, "shadows", "Z");
	roughnessField = Engine::Handler::GetField(cls, "roughness", "F");
	scaleField = Engine::Handler::GetField(cls, "scale", "Lengine/math/Float3;");
	rotationField = Engine::Handler::GetField(cls, "rotation", "Lengine/math/Float3;");
	positionField = Engine::Handler::GetField(cls, "position", "Lengine/math/Float3;");
	physicsField = Engine::Handler::GetField(cls, "physics", "Z");
	accelerField = Engine::Handler::GetField(cls, "acceler", "Lengine/math/Float3;");
	velocityField = Engine::Handler::GetField(cls, "velocity", "Lengine/math/Float3;");
	angularField = Engine::Handler::GetField(cls, "angular", "Lengine/math/Float3;");
}

// Data editors
void SetScriptFloat3(jobject field, const kl::float3& dat) {
	jclass objCls = Engine::Handler::env->GetObjectClass(field);
	jfieldID xField = Engine::Handler::GetField(objCls, "x", "F");
	jfieldID yField = Engine::Handler::GetField(objCls, "y", "F");
	jfieldID zField = Engine::Handler::GetField(objCls, "z", "F");
	Engine::Handler::env->SetFloatField(field, xField, dat.x);
	Engine::Handler::env->SetFloatField(field, yField, dat.y);
	Engine::Handler::env->SetFloatField(field, zField, dat.z);
}
void GetScriptFloat3(jobject field, kl::float3& dat) {
	jclass objCls = Engine::Handler::env->GetObjectClass(field);
	jfieldID xField = Engine::Handler::GetField(objCls, "x", "F");
	jfieldID yField = Engine::Handler::GetField(objCls, "y", "F");
	jfieldID zField = Engine::Handler::GetField(objCls, "z", "F");
	dat.x = Engine::Handler::env->GetFloatField(field, xField);
	dat.y = Engine::Handler::env->GetFloatField(field, yField);
	dat.z = Engine::Handler::env->GetFloatField(field, zField);
}
void Engine::Script::setEntityData(void* ent) {
	Engine::Entity* pEnt = (Engine::Entity*)ent;
	Engine::Handler::env->SetObjectField(inst, nameField, 
		Engine::Handler::env->NewString(
			(jchar*)kl::convert::toWString(pEnt->name).c_str(), jsize(pEnt->name.size())));
	Engine::Handler::env->SetBooleanField(inst, visibleField, pEnt->visible);
	Engine::Handler::env->SetBooleanField(inst, shadowsField, pEnt->shadows);
	Engine::Handler::env->SetFloatField(inst, roughnessField, pEnt->roughness);
	SetScriptFloat3(Engine::Handler::env->GetObjectField(inst, scaleField), pEnt->scale);
	SetScriptFloat3(Engine::Handler::env->GetObjectField(inst, rotationField), pEnt->rotation);
	SetScriptFloat3(Engine::Handler::env->GetObjectField(inst, positionField), pEnt->position);
	Engine::Handler::env->SetBooleanField(inst, physicsField, pEnt->physics);
	SetScriptFloat3(Engine::Handler::env->GetObjectField(inst, accelerField), pEnt->acceler);
	SetScriptFloat3(Engine::Handler::env->GetObjectField(inst, velocityField), pEnt->velocity);
	SetScriptFloat3(Engine::Handler::env->GetObjectField(inst, angularField), pEnt->angular);
}
void Engine::Script::getEntityData(void* ent) {
	Engine::Entity* pEnt = (Engine::Entity*)ent;
	pEnt->visible = Engine::Handler::env->GetBooleanField(inst, visibleField);
	pEnt->shadows = Engine::Handler::env->GetBooleanField(inst, shadowsField);
	pEnt->roughness = Engine::Handler::env->GetFloatField(inst, roughnessField);
	GetScriptFloat3(Engine::Handler::env->GetObjectField(inst, scaleField), pEnt->scale);
	GetScriptFloat3(Engine::Handler::env->GetObjectField(inst, rotationField), pEnt->rotation);
	GetScriptFloat3(Engine::Handler::env->GetObjectField(inst, positionField), pEnt->position);
	pEnt->physics = Engine::Handler::env->GetBooleanField(inst, physicsField);
	GetScriptFloat3(Engine::Handler::env->GetObjectField(inst, accelerField), pEnt->acceler);
	GetScriptFloat3(Engine::Handler::env->GetObjectField(inst, velocityField), pEnt->velocity);
	GetScriptFloat3(Engine::Handler::env->GetObjectField(inst, angularField), pEnt->angular);
}

// Method callers
void Engine::Script::callStart(void* ent) {
	setEntityData(ent);
	Engine::Handler::env->CallVoidMethod(inst, startMethod);
	getEntityData(ent);
}
void Engine::Script::callUpdate(void* ent) {
	setEntityData(ent);
	Engine::Handler::env->CallVoidMethod(inst, updateMethod);
	getEntityData(ent);
}
