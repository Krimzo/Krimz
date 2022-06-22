#include "Types/Named.h"


static std::set<std::string> existingNames[5] = {};

Engine::Named::Named(Named::Type type) : m_Type(type) {
	updateName("undefined");
}
Engine::Named::Named(Named::Type type, const std::string& name) : m_Type(type) {
	updateName(name);
}
Engine::Named::~Named() {
	existingNames[uint(m_Type)].erase(m_Name);
}

const std::string& Engine::Named::getName() const {
	return m_Name;
}
void Engine::Named::updateName(const std::string& newName) {
	std::string candName = newName;
	uint64 nameCounter = 0;
	existingNames[uint(m_Type)].erase(m_Name);
	while (existingNames[uint(m_Type)].contains(candName)) {
		candName = newName + std::to_string(++nameCounter);
	}
	existingNames[uint(m_Type)].insert(candName);
	m_Name = candName;
}
