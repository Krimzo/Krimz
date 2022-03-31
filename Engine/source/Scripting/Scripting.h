#pragma once

#include "Scripting/Handler.h"


namespace Engine {
	namespace Scripting {
		// Updates static time
		void UpdateTime();

		// Entities scripts callers
		void CallStarts();
		void CallUpdates();
	}
}
