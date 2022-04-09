#include "Scripting/Scripting.h"
#include "Utility/Time.h"
#include "Data/Entities.h"
#include "Logging/Logging.h"


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

// Script logging
void Engine::Scripting::HandleLogs()
{
	// Calling the flush method
	jobjectArray logBuff = (jobjectArray)Engine::JavaHandler::env->CallStaticObjectMethod(
		Engine::JavaHandler::loggerClass, Engine::JavaHandler::loggerFlushMethod);

	// Getting the log messages
	const int logBuffSize = Engine::JavaHandler::env->GetArrayLength(logBuff);
	for (int i = 0; i < logBuffSize; i++)
	{
		jstring logMess = (jstring)Engine::JavaHandler::env->GetObjectArrayElement(logBuff, i);
		Engine::log(Engine::JavaHandler::env->GetStringUTFChars(logMess, nullptr));
		Engine::JavaHandler::env->DeleteLocalRef(logMess);
	}

	// Cleanup
	Engine::JavaHandler::env->DeleteLocalRef(logBuff);
}
