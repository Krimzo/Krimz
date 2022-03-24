#include "Engine/Scripting/Scripting.h"
#include "Engine/Time/Time.h"
#include "Engine/Game/Game.h"


// Updates static time
void Engine::Scripting::UpdateTime() {
	Engine::Scripting::handler->env->SetStaticFloatField(Engine::Scripting::engineClass, deltaTField, Engine::Time::delta);
	Engine::Scripting::handler->env->SetStaticFloatField(Engine::Scripting::engineClass, elapsedTfield, Engine::Time::elapsed);
}

// Entities scripts callers
void Engine::Scripting::CallStarts() {
	for (int i = 0; i < Engine::Game::entities.size(); i++) {
		Engine::Game::entities[i]->callStarts();
	}
}
void Engine::Scripting::CallUpdates() {
	for (int i = 0; i < Engine::Game::entities.size(); i++) {
		Engine::Game::entities[i]->callUpdates();
	}
}
