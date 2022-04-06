#include "Scripting/JavaClass.h"


Engine::JavaClass::JavaClass(const std::string& name, jclass cls)
{
	this->name = name;
	this->cls = cls;
}
