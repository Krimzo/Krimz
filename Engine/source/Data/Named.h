#pragma once

#include "KrimzLib.h"


namespace Engine {
	class Named {
	public:
		String name;

		Named();
		Named(const String& name);
	};
}
