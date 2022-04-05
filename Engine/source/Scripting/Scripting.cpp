#include "Scripting/Scripting.h"
#include "Utility/Time.h"
#include "Data/Entities.h"


// Updates static time
void Engine::Scripting::UpdateTime()
{
	JavaHandler::env->SetStaticFloatField(JavaHandler::engineClass, JavaHandler::deltaTField, Time::delta);
	JavaHandler::env->SetStaticFloatField(JavaHandler::engineClass, JavaHandler::elapsedTField, Time::elapsed);
}

// Entities scripts callers
void Engine::Scripting::CallStarts()
{
	for (int i = 0; i < Engine::entities.size(); i++)
		Engine::entities[i]->callStarts();
}
void Engine::Scripting::CallUpdates()
{
	for (int i = 0; i < Engine::entities.size(); i++)
		Engine::entities[i]->callUpdates();
}
