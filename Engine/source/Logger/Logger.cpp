#include "Logger/Logger.h"


Krimz::Logger::Logger() {

}

Krimz::Logger::~Logger() {

}

void Krimz::Logger::clear() {
	m_Stream.clear();
	m_Counter = 0;
}
