#pragma once

#include "Scripting/JavaHandler.h"


namespace Krimz {
	namespace Scripting {
		void UpdateTime();

		void CallStarts();
		void CallUpdates();

		void HandleLogs();

		void UpdateInput();
		void HandleMousePos();
	}
}
