#include "Data/Named.h"


static std::set<String> existingNames[5] = {};

Engine::Named::Named(Named::Type type) : type(type) {
	updateName("undefined");
}
Engine::Named::Named(Named::Type type, const String& name) : type(type) {
	updateName(name);
}
Engine::Named::~Named() {
	existingNames[int(type)].erase(name);
}

const String& Engine::Named::getName() const {
	return name;
}
void Engine::Named::updateName(const String& newName) {
	String candName = newName;
	size_t nameCounter = 0;
	existingNames[int(type)].erase(name);
	while (existingNames[int(type)].contains(candName)) {
		candName = newName + std::to_string(++nameCounter);
	}
	existingNames[int(type)].insert(candName);
	name = candName;
}
