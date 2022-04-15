#pragma once

#include "Scripting/JavaHandler.h"


namespace Engine
{
	namespace Scripting
	{
		// Updates static time
		void UpdateTime();

		// Entities scripts callers
		void CallStarts();
		void CallUpdates();

		// Script logging
		void HandleLogs();

		// Input
		void UpdateInput();
		void HandleMousePos();
	}
}
