#pragma once

#include <string>
#include <jni.h>

#include "KrimzLib.h"


namespace Engine {
	class JavaClass {
	public:
		String name;
		jclass cls = nullptr;

		JavaClass(const String& name, jclass cls);
	};
}
