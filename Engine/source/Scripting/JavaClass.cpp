#include "Scripting/JavaClass.h"


Engine::JavaClass::JavaClass(const std::string& name, jclass cls)
	: name(name), cls(cls)
{
}
