#pragma once

#include <iostream>
#include <jni.h>


namespace Engine {
	class Script {
	public:
		String path;
		jobject inst;

		// Methods
		jmethodID startMethod = nullptr;
		jmethodID updateMethod = nullptr;

		Script(const String& filePath);
		Script(const Engine::Script&) = delete;
		void operator=(const Engine::Script&) = delete;
		~Script();

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
