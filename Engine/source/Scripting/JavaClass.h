#pragma once

#include <string>
#include <jni.h>


namespace Engine
{
	class JavaClass
	{
	public:
		std::string name;
		jclass cls = nullptr;

		JavaClass(const std::string& name, jclass cls);
	};
}
