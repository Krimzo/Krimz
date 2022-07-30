#pragma once

#include "KrimzLib.h"


namespace Krimz {
	class Logger {
		std::stringstream m_Stream = {};
		uint64 m_Counter = 0;

	public:
		Logger();
		Logger(const Logger&) = delete;
		void operator=(const Logger&) = delete;
		~Logger();

		template<typename T>
		void log(const T& data) {
			kl::print(m_Stream, ++m_Counter, ". [", kl::time::date(), "] -> ", data);
		}

		void clear();
	};
}
