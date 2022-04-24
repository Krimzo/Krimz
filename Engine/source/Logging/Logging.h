#pragma once

#include <vector>
#include <string>

#include "KrimzLib.h"


namespace Engine {
	void log(int dat);
	void log(float dat);
	void log(const String& dat);

	namespace Logging {
		inline uint64_t logCounter = 0;
		inline uint64_t logUnseenCount = 0;
		inline std::vector<String> logs;
	}
}
