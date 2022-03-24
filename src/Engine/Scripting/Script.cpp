#include "Engine/Scripting/Script.h"
#include "Engine/Game/Entity.h"
#include "Engine/Time/Time.h"


Engine::Script::Script(jclass cls) {
	// Instance
	inst = Engine::Scripting::handler->newInst(cls, Engine::Scripting::handler->getMethod(cls, "<init>", "()V"));

	// Methods
	startMethod = Engine::Scripting::handler->getMethod(cls, "start", "()V");
	updateMethod = Engine::Scripting::handler->getMethod(cls, "update", "()V");

	// Fields
	nameField = Engine::Scripting::handler->getField(cls, "name", "Ljava/lang/String;");
	visibleField = Engine::Scripting::handler->getField(cls, "visible", "Z");
	shadowsField = Engine::Scripting::handler->getField(cls, "shadows", "Z");
	roughnessField = Engine::Scripting::handler->getField(cls, "roughness", "F");
	sizeField = Engine::Scripting::handler->getField(cls, "size", "Lengine/Float3;");
	rotationField = Engine::Scripting::handler->getField(cls, "rotation", "Lengine/Float3;");
	positionField = Engine::Scripting::handler->getField(cls, "position", "Lengine/Float3;");
	physicsField = Engine::Scripting::handler->getField(cls, "physics", "Z");
	accelerField = Engine::Scripting::handler->getField(cls, "acceler", "Lengine/Float3;");
	velocityField = Engine::Scripting::handler->getField(cls, "velocity", "Lengine/Float3;");
	angularField = Engine::Scripting::handler->getField(cls, "angular", "Lengine/Float3;");
}

// Data editors
void SetScriptFloat3(jobject field, const kl::float3& dat) {
	jclass objCls = Engine::Scripting::handler->env->GetObjectClass(field);
	jfieldID xField = Engine::Scripting::handler->getField(objCls, "x", "F");
	jfieldID yField = Engine::Scripting::handler->getField(objCls, "y", "F");
	jfieldID zField = Engine::Scripting::handler->getField(objCls, "z", "F");
	Engine::Scripting::handler->env->SetFloatField(field, xField, dat.x);
	Engine::Scripting::handler->env->SetFloatField(field, yField, dat.y);
	Engine::Scripting::handler->env->SetFloatField(field, zField, dat.z);
}
void GetScriptFloat3(jobject field, kl::float3& dat) {
	jclass objCls = Engine::Scripting::handler->env->GetObjectClass(field);
	jfieldID xField = Engine::Scripting::handler->getField(objCls, "x", "F");
	jfieldID yField = Engine::Scripting::handler->getField(objCls, "y", "F");
	jfieldID zField = Engine::Scripting::handler->getField(objCls, "z", "F");
	dat.x = Engine::Scripting::handler->env->GetFloatField(field, xField);
	dat.y = Engine::Scripting::handler->env->GetFloatField(field, yField);
	dat.z = Engine::Scripting::handler->env->GetFloatField(field, zField);
}
void Engine::Script::setData(void* ent) {
	Engine::Game::Entity* pEnt = (Engine::Game::Entity*)ent;
	Engine::Scripting::handler->env->SetBooleanField(inst, visibleField, pEnt->visible);
	Engine::Scripting::handler->env->SetBooleanField(inst, shadowsField, pEnt->shadows);
	Engine::Scripting::handler->env->SetFloatField(inst, roughnessField, pEnt->roughness);
	SetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, sizeField), pEnt->size);
	SetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, rotationField), pEnt->rotation);
	SetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, positionField), pEnt->position);
	Engine::Scripting::handler->env->SetBooleanField(inst, physicsField, pEnt->physics);
	SetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, accelerField), pEnt->acceler);
	SetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, velocityField), pEnt->velocity);
	SetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, angularField), pEnt->angular);
}
void Engine::Script::getData(void* ent) {
	Engine::Game::Entity* pEnt = (Engine::Game::Entity*)ent;
	pEnt->visible = Engine::Scripting::handler->env->GetBooleanField(inst, visibleField);
	pEnt->shadows = Engine::Scripting::handler->env->GetBooleanField(inst, shadowsField);
	pEnt->roughness = Engine::Scripting::handler->env->GetFloatField(inst, roughnessField);
	GetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, sizeField), pEnt->size);
	GetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, rotationField), pEnt->rotation);
	GetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, positionField), pEnt->position);
	pEnt->physics = Engine::Scripting::handler->env->GetBooleanField(inst, physicsField);
	GetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, accelerField), pEnt->acceler);
	GetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, velocityField), pEnt->velocity);
	GetScriptFloat3(Engine::Scripting::handler->env->GetObjectField(inst, angularField), pEnt->angular);
}

// Destroys the instnace
void Engine::Script::destroy() {
	Engine::Scripting::handler->delInst(inst);
}

// Method callers
void Engine::Script::callStart(void* ent) {
	setData(ent);
	Engine::Scripting::handler->env->CallVoidMethod(inst, startMethod);
	getData(ent);
}
void Engine::Script::callUpdate(void* ent) {
	setData(ent);
	Engine::Scripting::handler->env->CallVoidMethod(inst, updateMethod);
	getData(ent);
}
