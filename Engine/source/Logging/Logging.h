#pragma once

#include "KrimzLib.h"


namespace Engine {
	namespace Logging {
		inline uint64 logCounter = 0;
		inline uint64 logUnseenCount = 0;
		inline std::vector<std::string> logs;
	}

	template<typename T> inline void log(const T& data) {
		std::time_t time = std::time(nullptr);
		std::tm now = {};
		localtime_s(&now, &time);
		std::stringstream ss;
		ss << ++Engine::Logging::logCounter <<
			". [" << (now.tm_year + 1900) <<
			"." << (now.tm_mon + 1) <<
			"." << now.tm_mday <<
			"-" << now.tm_hour <<
			":" << now.tm_min <<
			":" << now.tm_sec <<
			"] -> " << data;
		Engine::Logging::logs.push_back(ss.str());
		Engine::Logging::logUnseenCount++;
	}
}
