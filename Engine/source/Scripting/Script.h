#pragma once

#include <iostream>
#include <jni.h>


namespace Engine {
	class Script {
	public:
		// File info
		std::string name;
		std::string path;

		// Instance
		jobject inst = nullptr;

		// Methods
		jmethodID startMethod = nullptr;
		jmethodID updateMethod = nullptr;

		// Fields
		jfieldID nameField = nullptr;
		jfieldID visibleField = nullptr;
		jfieldID shadowsField = nullptr;
		jfieldID roughnessField = nullptr;
		jfieldID sizeField = nullptr;
		jfieldID rotationField = nullptr;
		jfieldID positionField = nullptr;
		jfieldID physicsField = nullptr;
		jfieldID accelerField = nullptr;
		jfieldID velocityField = nullptr;
		jfieldID angularField = nullptr;

		Script(const std::string& name, const std::string& filePath);
		~Script();

		// Reloads byte data
		void reload();

		// Data editors
		void setData(void* ent);
		void getData(void* ent);

		// Method callers
		void callStart(void* ent);
		void callUpdate(void* ent);
	};
}
