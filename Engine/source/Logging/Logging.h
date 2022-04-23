#pragma once

#include <vector>
#include <string>


namespace Engine {
	void log(int dat);
	void log(float dat);
	void log(const std::string& dat);

	namespace Logging {
		inline uint64_t logCounter = 0;
		inline uint64_t logUnseenCount = 0;
		inline std::vector<std::string> logs;
	}
}
