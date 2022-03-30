#include "Scripting/Scripting.h"
#include "Utility/Time.h"
#include "Game/Game.h"


// Updates static time
void Engine::Scripting::UpdateTime() {
	Engine::Scripting::handler->env->SetStaticFloatField(Engine::Scripting::engineClass, deltaTField, Engine::Time::delta);
	Engine::Scripting::handler->env->SetStaticFloatField(Engine::Scripting::engineClass, elapsedTField, Engine::Time::elapsed);
}

// Entities scripts callers
void Engine::Scripting::CallStarts() {
	for (int i = 0; i < Engine::entities.size(); i++) {
		Engine::entities[i]->callStarts();
	}
}
void Engine::Scripting::CallUpdates() {
	for (int i = 0; i < Engine::entities.size(); i++) {
		Engine::entities[i]->callUpdates();
	}
}
