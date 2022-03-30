#pragma once

#include <vector>
#include <string>


namespace Engine {
	void log(int dat);
	void log(float dat);
	void log(const std::string& dat);

	namespace Logging {
		inline std::vector<std::string> logs;
	}
}
