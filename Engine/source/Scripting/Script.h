#pragma once

#include <iostream>
#include <jni.h>


namespace Engine {
	class Script {
	public:
		std::string path;
		jobject inst = nullptr;

		// Methods
		jmethodID startMethod = nullptr;
		jmethodID updateMethod = nullptr;

		Script(const std::string& filePath);

		// Reloads bytes
		void reload();

		// Data editors
		void setEntityData(void* ent);
		void getEntityData(void* ent);

		// Method callers
		void callStart(void* ent);
		void callUpdate(void* ent);
	};
}
