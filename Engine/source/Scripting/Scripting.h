#pragma once

#include "Scripting/ScriptHandler.h"


namespace Engine {
	namespace Scripting {
		inline ScriptHandler* handler = nullptr;
		inline jclass engineClass = nullptr;
		inline jfieldID deltaTField = nullptr;
		inline jfieldID elapsedTField = nullptr;

		// Updates static time
		void UpdateTime();

		// Entities scripts callers
		void CallStarts();
		void CallUpdates();
	}
}
