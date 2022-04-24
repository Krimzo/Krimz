#include "Logging/Logging.h"
#include <iomanip>
#include <sstream>
#include <ctime>


void Engine::log(int dat) {
	Engine::log(std::to_string(dat));
}

void Engine::log(float dat) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << dat;
	Engine::log(ss.str());
}

void Engine::log(const String& dat) {
	std::time_t t = time(nullptr);
	std::tm now = {};
	localtime_s(&now, &t);
	std::stringstream ss;
	ss << ++Engine::Logging::logCounter << ". [" << (now.tm_year + 1900) << "." << (now.tm_mon + 1) << "." << now.tm_mday << "-" << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "] -> ";
	Engine::Logging::logs.push_back(ss.str() + dat);
	Engine::Logging::logUnseenCount++;
}
